#include "EventCollector.hpp"



void EventCollector::CollectKeyEvent(KeyEventType newEvent)
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

void EventCollector::CollectMousPosition(float x, float y)
{
	mouse_X = x;
	mouse_Y = y;
}

void EventCollector::Flush()
{
	events.resize(0);
	mouse_X = 0;
	mouse_Y = 0;
}

