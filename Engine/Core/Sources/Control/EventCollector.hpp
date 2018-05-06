#pragma once

#include <vector>

#include "ControlTypes.hpp"


struct EventCollector
{
public:
	std::vector<KeyEventType> events;
	float mouse_X;
	float mouse_Y;

public:

	void CollectKeyEvent	(KeyEventType newEvent);
	void CollectMousPosition(float x, float y);

public:

	void Flush();
};
