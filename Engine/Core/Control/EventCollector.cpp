#include "EventCollector.hpp"



void EventCollecor::CollectKeyEvent(KeyEvent newEvent)
{
	for (auto& event : events)
	{
		if (event == newEvent)
		{
			return;
		}
	}
	events.emplace_back(newEvent);
}

void EventCollecor::CollectMousPosition(float x, float y)
{
	mouse_X = x;
	mouse_Y = y;
}

const std::vector<KeyEvent>& EventCollecor::GetEvents() const
{
	return events;
}

float EventCollecor::GetMouseX() const
{
	return mouse_X;
}

float EventCollecor::GetMouseY() const
{
	return mouse_Y;
}

void EventCollecor::Flush()
{
	events.resize(0);
	mouse_X = 0;
	mouse_Y = 0;
}

