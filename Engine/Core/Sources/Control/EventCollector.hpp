#pragma once

#include <vector>

#include "ControlTypes.hpp"


struct EventCollector
{
public:
	std::vector<KeyEvent> events;
	float mouse_X;
	float mouse_Y;

public:

	void CollectKeyEvent	(KeyEvent newEvent);
	void CollectMousPosition(float x, float y);

public:

	void Flush();
};
