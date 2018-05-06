#pragma once

#include <vector>
#include <unordered_map>

#include "EventBinder.hpp"




/**
 */
class EventManager
{
	using AxisID = size_t;

public:

	EventManager();

public: //~~~~~~~~~~~~~~~~~~~| setup

	void ProcessEventBinder(EventBinder& binder);

	void AddAxis  (std::string name, std::vector<ControlAxisPair> keys);
	void AddAction(std::string name, ControlKey key);

public: //~~~~~~~~~~~~~~~~~~~| update

	void ProcessKeys(const std::vector<KeyEvent>& events);
	void UpdateMouseLocation(float x, float y);

protected:

	void ProcessActions(EventBinder& binder);
	void ProcessAxis   (EventBinder& binder);

	ControlAction* FindAction(const std::string& name);
	ControlAxis*   FindAxis  (const std::string& name);

	bool SetStates(ControlKey key);

protected:

	/// >> control types
	std::vector<ControlAction> registeredActions;
	std::vector<ControlAxis>   registeredAxis;
	///

	std::unordered_map<ControlKey, EKeyAction> keyStatus;

	/// >> axis
	AxisID axisID;
	std::unordered_map<ControlKey, AxisID> keyToAxisID;
	std::unordered_map<ControlKey, float > axisValue;
	std::unordered_map<AxisID	 , float > axisSummary;
	std::unordered_map<AxisID	 , bool	 > axisbDone;
	std::unordered_map<AxisID	 , BindedAxis::Callback> axis_map;
	/// << 

	/// >> actions
	std::unordered_map<ControlKey, BindedActions::Callback> action_map;
	///

	/// >> mouse
	float mouse_X;
	float mouse_Y;
	/// <<
};