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
	IRunable* task;
	bool	  bDie;

	ThreadTask(IRunable* task = nullptr, bool bDie = true)
		: task(task)
		, bDie(bDie)
	{}

	bool operator==(const ThreadTask& r)
	{
		 return task == r.task
			 && bDie == r.bDie;
	}

public:

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