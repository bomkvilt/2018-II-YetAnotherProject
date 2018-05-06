#pragma once

#include <thread>
#include "Common.hpp"
#include "Runable.hpp"

using ThreadID = std::thread::id;


class Thread final
{
public:
	// Creates a new thread
	static UNIQUE(Thread) Get();
	static ThreadID GetThreadID();

public:
	void Run();
};