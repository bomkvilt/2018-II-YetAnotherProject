#pragma once

#include "Control\ControlTypes.hpp"
#include "SFML/Graphics.hpp"
#include "Control/EventCollector.hpp"


class KeyHandler_base : public sf::Event
{
public:

	KeyHandler_base(EventCollector& collector);

protected:

	void SetKeyMode (const sf::Event& ega, KeyEventType& event);
	void SetMousePos(const sf::Event& ega);

	void Store(KeyEventType event);

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

	virtual bool handle(const sf::Event& ega) ;

};


/**
 */
class KeyboardHandler : public KeyHandler_base
{
public:

	KeyboardHandler(EventCollector& collector);

	virtual bool handle(const sf::Event& ega) ;

};

