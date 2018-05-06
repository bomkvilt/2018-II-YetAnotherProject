#pragma once

#include <functional>



/**
 */
struct IRunable
{
public:

	virtual void Run() = 0;

public:

	virtual ~IRunable() {};
};



/**
 */
struct ThreadTask
{
public:
	enum Type
	{
		  eNextLoop     // Task found. But a next toop is required
		, eShouldDie    // Reques passed. The thrad must be stopped
		, eTaskFound    // A task is found. 
		, eNoTasksFound // No Task is found. Weight for a new task
	};

public:
	IRunable* task;
	Type      type;

	ThreadTask()
		: task(nullptr)
		, type(eTaskFound)
	{}

	ThreadTask(IRunable* task, Type type = eTaskFound)
		: task(task)
		, type(type)
	{}

	bool operator==(const ThreadTask& r)
	{
		 return task == r.task
			 && type == r.type;
	}

	bool operator!=(const ThreadTask& r)
	{
		return !(*this == r);
	}

public:

	const static ThreadTask NoTasksFound;
	const static ThreadTask ShouldDie;
	const static ThreadTask NextLoop;
};


/** default IRunable realisation
 */
class Runable : public IRunable
{
public:

	template<class _Fn>
	Runable(_Fn& function)
		: runner(function)
	{}

	template<class _Fn, class ...Args>
	Runable(_Fn& function, Args&&... args)
		: runner(function, std::forward(args))
	{}

	virtual void Run() override
	{
		runner();
	}

	std::function<void()> runner;
};