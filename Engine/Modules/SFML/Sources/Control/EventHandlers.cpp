#include "EventHandlers.hpp"

#include <assert.h>
#include <iostream>


/********************************************************************
*								//
********************************************************************/

KeyHandler_base::KeyHandler_base(EventCollector& collector)
	: collector(collector)
{}

void KeyHandler_base::SetKeyMode(const sf::Event& ega, KeyEventType& event)
{
	switch (ega.type) {
	case sf::Keyboard::LAlt				:	event.modifier = EKeyModifier::eAlt;	break;
	case sf::Keyboard::LControl			:	event.modifier = EKeyModifier::eCtrl;	break;
	case sf::Keyboard::LShift			:	event.modifier = EKeyModifier::eShift;	break;
	}
}

void KeyHandler_base::SetMousePos(const  sf::Event& ega)
{
}

void KeyHandler_base::Store(KeyEventType event)
{
	collector.CollectKeyEvent(event);
}


/********************************************************************
*								Mouse
********************************************************************/

MouseHandler::MouseHandler(EventCollector& collector)
	: KeyHandler_base(collector)
{}

bool MouseHandler::handle(const sf::Event& ega)
{
	auto eventType = ega.type;
	

	KeyEventType event;

	switch (eventType) {
	case sf::Event::MouseButtonPressed:  event.action = BA_Pressed;		break; 
	case sf::Event::MouseButtonReleased				:	event.action = BA_Released;		break;
	case sf::Event::Closed							:   std::cout << "close button";
	case sf::Event::MouseMoved						:	SetMousePos(ega);				return false; //TODO:
	case sf::Event::MouseWheelMoved					:			return false; //TODO:
	default: return false;
	}

		
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) event.key = KEY_MOUSE_L;	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) event.key = KEY_MOUSE_R;
	

	SetKeyMode(ega, event);
	Store(event);

	return true;
}

/********************************************************************
*								Keyboar
********************************************************************/


KeyboardHandler::KeyboardHandler(EventCollector& collector)
	: KeyHandler_base(collector)
{}

bool KeyboardHandler::handle(const sf::Event& ega)
{
	auto eventType = ega.type;
	int button = ega.key.code;
	
	KeyEventType event;
	switch (eventType) {
	case sf::Event::KeyPressed		:	event.action = EKeyAction::BA_Pressed;  break;
	case sf::Event::KeyReleased		:	event.action = EKeyAction::BA_Released; break;
	default: return false;
	}
	

	switch (button) {
	case sf::Keyboard::Key::Down		:	 break;
	case sf::Keyboard::Key::Up			:	 break;
	case sf::Keyboard::Key::Return		:	event.key = KEY_ENTER;		break;
	case sf::Keyboard::Key::Tab			:	event.key = KEY_TAB;		break;
	case sf::Keyboard::Key::Escape		:	event.key = KEY_ESCAPE;		break;
	
	default:
		switch (button) {
		case sf::Keyboard::Key::R:			button = 114; event.key = (EInputKey)button; break;
		case sf::Keyboard::Key::F:			button = 102;	event.key = (EInputKey)button; break;
	}
		
		
	}
	event.rawKey = button;

	SetKeyMode(ega, event);
	Store(event);

	return true;
}


/********************************************************************
*								//
********************************************************************/
