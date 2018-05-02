#pragma once

#include <thread>
#include <memory>

#include "Misc.hpp"

#include "Runable.hpp"

using ThreadID = std::thread::id;


class Thread final
{
public:
	         Thread();
	virtual ~Thread();

	// Creates a new thread
	static UNIQUE(Thread) Get();
	static ThreadID GetThreadID();

public:
	void Run();

private:
	bool bAcive;
};