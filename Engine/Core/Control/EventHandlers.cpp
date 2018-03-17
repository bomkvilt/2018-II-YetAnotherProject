#include "EventHandlers.hpp"

#include <assert.h>
#include <iostream>


/********************************************************************
*								//
********************************************************************/

KeyHandler_base::KeyHandler_base(EventCollecor& collector)
	: collector(collector)
{}

void KeyHandler_base::SetKeyMode(const osgGA::GUIEventAdapter& ega, KeyEvent& event)
{
	switch (ega.getModKeyMask()) {
	case osgGA::GUIEventAdapter::MODKEY_ALT			:	event.modifier = EKeyModifier::eAlt;	break;
	case osgGA::GUIEventAdapter::MODKEY_CTRL		:	event.modifier = EKeyModifier::eCtrl;	break;
	case osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT	:
	case osgGA::GUIEventAdapter::MODKEY_RIGHT_SHIFT	:
	case osgGA::GUIEventAdapter::MODKEY_SHIFT		:	event.modifier = EKeyModifier::eShift;	break;
	}
}

void KeyHandler_base::SetMousePos(const osgGA::GUIEventAdapter & ega)
{
	float x = (ega.getX()-ega.getXmin()) / (ega.getXmax()-ega.getXmin()) * ega.getWindowWidth ();
	float y = (ega.getY()-ega.getYmin()) / (ega.getYmax()-ega.getYmin()) * ega.getWindowHeight();
	collector.CollectMousPosition(x, y);
}

void KeyHandler_base::Store(KeyEvent event)
{
	collector.CollectKeyEvent(event);
}


/********************************************************************
*								Mouse
********************************************************************/

MouseHandler::MouseHandler(EventCollecor& collector)
	: KeyHandler_base(collector)
{}

bool MouseHandler::handle(const osgGA::GUIEventAdapter& ega, osgGA::GUIActionAdapter& aa)
{
	auto eventType = ega.getEventType();
	int button	   = ega.getButton();

	KeyEvent event;

	switch (eventType) {
	case osgGA::GUIEventAdapter::PUSH				:	event.action = BA_Pressed;		break;
	case osgGA::GUIEventAdapter::RELEASE			:	event.action = BA_Released;		break;
	case osgGA::GUIEventAdapter::DOUBLECLICK		:	event.action = BA_DoubleClick;	break;

	case osgGA::GUIEventAdapter::DRAG				:	SetMousePos(ega);				return false; //TODO:
	case osgGA::GUIEventAdapter::MOVE				:	SetMousePos(ega);				return false; //TODO:
	case osgGA::GUIEventAdapter::SCROLL				:									return false; //TODO:
	default: return false;
	}

	switch (button) {	
	case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON	: event.key = KEY_MOUSE_L;	break;
	case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON	: event.key = KEY_MOUSE_R;	break;
	}

	SetKeyMode(ega, event);
	Store(event);

	return true;
}

/********************************************************************
*								Keyboar
********************************************************************/


KeyboardHandler::KeyboardHandler(EventCollecor& collector)
	: KeyHandler_base(collector)
{}

bool KeyboardHandler::handle(const osgGA::GUIEventAdapter& ega, osgGA::GUIActionAdapter& aa)
{
	auto eventType = ega.getEventType();
	int button	   = ega.getKey();

	KeyEvent event;

	switch (eventType) {
	case osgGA::GUIEventAdapter::KEYDOWN			:	event.action = EKeyAction::BA_Pressed;  break;
	case osgGA::GUIEventAdapter::KEYUP				:	event.action = EKeyAction::BA_Released; break;
	default: return false;
	}

	switch (button) {
	case osgGA::GUIEventAdapter::KEY_KP_Enter		:	event.key = KEY_ENTER;		break;
	case osgGA::GUIEventAdapter::KEY_Tab			:
	case osgGA::GUIEventAdapter::KEY_KP_Tab			:	event.key = KEY_TAB;		break;
	case osgGA::GUIEventAdapter::KEY_Escape			:	event.key = KEY_ESCAPE;		break;
	case osgGA::GUIEventAdapter::KEY_Caps_Lock		:	event.key = KEY_CAPS;		break;
	default:
		if (button < 128) //TODO: make valid binding
		{
			event.key = (EInputKey)button;
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
