#include "EventManager.hpp"
#include <iostream>

EventManager::EventManager()
	: axisID(0)
{
}

void EventManager::ProcessEventBinder(EventBinder& binder)
{
	ProcessActions(binder);
	ProcessAxis   (binder);
}

void EventManager::AddAxis(std::string name, std::vector<ControlAxisPair> keys)
{
	registeredAxis.emplace_back(std::move(name), std::move(keys));
}

void EventManager::AddAction(std::string name, ControlKey key)
{
	registeredActions.emplace_back(std::move(name), key);
}

void EventManager::ProcessKeys(const std::vector<KeyEvent>& events)
{
	for (auto& event : events)
	{
		ControlKey key = event.GetControlKey();
		if (!SetStates(key))
		{
			continue;
		}

		auto actionPos = action_map.find(key);
		auto actionEnd = action_map.end();
		if ( actionPos != actionEnd)
		{
			actionPos->second(key.action);
			continue;
		}

		auto baseKey = key.getKey();
		auto axisPos = keyToAxisID.find(baseKey);
		auto axisEnd = keyToAxisID.end();
		if ( axisPos != axisEnd )
		{
			AxisID ID = axisPos->second;
			float value = axisValue[baseKey];
			
			int factor = key.action == EKeyAction::BA_Pressed ?  1 : -1;
			axisSummary[ID] += value * factor;
			axisbDone  [ID] = true;
			
			axis_map[ID](axisSummary[ID]);

			continue;
		}
	}

	for (auto& ID_Done : axisbDone)
	{
		AxisID  ID = ID_Done.first;
		bool& done = ID_Done.second;

		if (!done)
		{
			axis_map[ID](axisSummary[ID]);
		}
		else done = false;
	}
}

void EventManager::UpdateMouseLocation(float x, float y)
{
	mouse_X = x;
	mouse_Y = y;
}


void EventManager::ProcessActions(EventBinder& binder)
{
	for (auto& binded : binder.bindedActions)
	{
		auto* action = FindAction(binded.name);
		if (!action) continue;
		
		ControlKey key = action->key;
		key.action = binded.type == BA_Pressed
			? EKeyAction::BA_Pressed
			: EKeyAction::BA_Released;
		if (key) continue;

		action_map[key] = binded.calback;
	}
}

void EventManager::ProcessAxis(EventBinder& binder)
{
	for (auto& binded : binder.bindedAxis)
	{
		auto* axis = FindAxis(binded.name);
		if (!axis) continue;

		AxisID currentID = axisID++;

		for (auto& button : axis->axis)
		{
			float	 value = button.value;
			ControlKey key = button.key;

			keyToAxisID[key] = currentID;
			axisValue  [key] = value;
		}
		axisSummary[currentID] = 0;

		axis_map[currentID] = binded.callback;
	}
}

ControlAction* EventManager::FindAction(const std::string& name)
{
	for (auto& action : registeredActions)
	{
		if (action.name == name)
		{
			return &action;
		}
	}
	return nullptr;
}

ControlAxis* EventManager::FindAxis(const std::string& name)
{
	for (auto& axis : registeredAxis)
	{
		if (axis.name == name)
		{
			return &axis;
		}
	}
	return nullptr;
}

bool EventManager::SetStates(ControlKey key)
{
	ControlKey baseKey = key;
	baseKey.action = BA_None;

	EKeyAction currentAction = keyStatus[baseKey];
	if (currentAction == key.action)
	{
		return false;
	}
	keyStatus[baseKey] = key.action;

	return true;
}
