#pragma once

#include <osgGA/GUIEventHandler>

#include "EventCollector.hpp"

//***********
//***********	TODO: make base handler class to handle modifiers in only place
//***********


class KeyHandler_base : public osgGA::GUIEventHandler
{
public:

	KeyHandler_base(EventCollecor& collector);

protected:

	void SetKeyMode (const osgGA::GUIEventAdapter& ega, KeyEvent& event);
	void SetMousePos(const osgGA::GUIEventAdapter& ega);

	void Store(KeyEvent event);

protected:

	EventCollecor& collector;
};

/**
*/
class MouseHandler  : public KeyHandler_base
{
	typedef bool (MouseHandler::*MouseAction)(float, float, int);

public:

	MouseHandler(EventCollecor& collector);

	virtual bool handle(const osgGA::GUIEventAdapter& ega, osgGA::GUIActionAdapter& aa) override;

};


/**
 */
class KeyboardHandler : public KeyHandler_base
{
public:

	KeyboardHandler(EventCollecor& collector);

	virtual bool handle(const osgGA::GUIEventAdapter& ega, osgGA::GUIActionAdapter& aa) override;

};


/**
 */
class ResizeHandler
{

};

/**
*/
class CameraSwitchHandler
{

};

/**
*/
class StatsHandler
{

};

/**
*/
class WindowSizeHandler
{

};

/**
*/
class StateSetManipulator
{

};