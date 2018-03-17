#pragma once

#include <vector>

#include "ControlTypes.hpp"


class EventCollecor
{
public:

	void CollectKeyEvent	(KeyEvent newEvent);
	void CollectMousPosition(float x, float y);

public:

	const std::vector<KeyEvent>& GetEvents() const;

	float GetMouseX() const;
	float GetMouseY() const;

	void Flush();

protected:

	std::vector<KeyEvent> events;

	float mouse_X;
	float mouse_Y;
};
