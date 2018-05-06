#pragma once

#include "ControlTypes.hpp"


/** A class is being used to bind a player
 * input on a function in up controlled avatar
 */
class EventBinder
{
	friend class EventManager;

public:

	template<class _T, typename _Fn>
	void BindAction(const std::string& action, EKeyAction type, _T* target, _Fn function)
	{
		bindedActions.emplace_back(action, type, BindedActions::Callback(
			std::bind(function, target, std::placeholders::_1)
			));
	}

	template<class _T, typename _Fn>
	void BindAxis(const std::string& axis, _T* target, _Fn function)
	{
		bindedAxis.emplace_back(axis, BindedAxis::Callback(
			std::bind(function, target, std::placeholders::_1)
			));
	}

protected:

	std::vector<BindedActions> bindedActions;
	std::vector<BindedAxis>	   bindedAxis;
};
