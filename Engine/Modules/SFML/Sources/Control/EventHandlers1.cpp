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

		case sf::Keyboard::Key::A:	event.key = KEY_A; break;
		case sf::Keyboard::Key::B:	event.key = KEY_B; break;
		case sf::Keyboard::Key::C:	event.key = KEY_C; break;
		case sf::Keyboard::Key::D:	event.key = KEY_D; break;
		case sf::Keyboard::Key::E:	event.key = KEY_E; break;
		case sf::Keyboard::Key::F:	event.key = KEY_F; break;
		case sf::Keyboard::Key::G:	event.key = KEY_G; break;
		case sf::Keyboard::Key::H:	event.key = KEY_H; break;
		case sf::Keyboard::Key::I:	event.key = KEY_I; break;
		case sf::Keyboard::Key::J:	event.key = KEY_J; break;
		case sf::Keyboard::Key::K:	event.key = KEY_K; break;
		case sf::Keyboard::Key::L:	event.key = KEY_L; break;
		case sf::Keyboard::Key::M:	event.key = KEY_M; break;
		case sf::Keyboard::Key::N:	event.key = KEY_N; break;
		case sf::Keyboard::Key::O:	event.key = KEY_O; break;
		case sf::Keyboard::Key::P:	event.key = KEY_P; break;
		case sf::Keyboard::Key::Q:	event.key = KEY_Q; break;
		case sf::Keyboard::Key::R:	event.key = KEY_R; break;
		case sf::Keyboard::Key::S:	event.key = KEY_S; break;
		case sf::Keyboard::Key::T:	event.key = KEY_T; break;
		case sf::Keyboard::Key::U:	event.key = KEY_U; break;
		case sf::Keyboard::Key::V:	event.key = KEY_V; break;
		case sf::Keyboard::Key::W:	event.key = KEY_W; break;
		case sf::Keyboard::Key::X:	event.key = KEY_X; break;
		case sf::Keyboard::Key::Y:	event.key = KEY_Y; break;
		case sf::Keyboard::Key::Z:	event.key = KEY_Z; break;

		}


	}


	SetKeyMode(ega, event);
	Store(event);

	return true;
}


/********************************************************************
*								//
********************************************************************/
