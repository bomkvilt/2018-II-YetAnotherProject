#pragma once

#include <thread>
#include <memory>

#include "../../Misc.hpp"

#include "Runable.hpp"

using ThreadID = std::thread::id;

/** Thread 
 *	The thread:
 *	. takes a free task from ThreadPull
 *	. process the selected task
 *	. goes to the TreadPull for the next one
 * 
 *	NOTE:
 *	. to stop the thread TP should send him 'bDie == 1'
 *	
 *	see:
 *	@ThreadTask
 *	@ThreadPull
 */
class Thread
{
public:

	Thread();
	virtual ~Thread();

	/** Creates a new thread */
	static UNIQUE(Thread) Get();

public:

	static ThreadID GetThreadID();

	void Run();



	bool bAcive;
};