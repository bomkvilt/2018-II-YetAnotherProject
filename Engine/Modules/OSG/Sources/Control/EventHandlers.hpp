#pragma once

#include <osgGA/GUIEventHandler>

#include "Control/EventCollector.hpp"


class KeyHandler_base : public osgGA::GUIEventHandler
{
public:

	KeyHandler_base(EventCollector& collector);

protected:

	void SetKeyMode (const osgGA::GUIEventAdapter& ega, KeyEvent& event);
	void SetMousePos(const osgGA::GUIEventAdapter& ega);

	void Store(KeyEvent event);

protected:

	EventCollector& collector;
};

/**
*/
class MouseHandler  : public KeyHandler_base
{
	typedef bool (MouseHandler::*MouseAction)(float, float, int);

public:

	MouseHandler(EventCollector& collector);

	virtual bool handle(const osgGA::GUIEventAdapter& ega, osgGA::GUIActionAdapter& aa) override;

};


/**
 */
class KeyboardHandler : public KeyHandler_base
{
public:

	KeyboardHandler(EventCollector& collector);

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