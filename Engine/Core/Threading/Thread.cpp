#include "Thread.hpp"

#include <chrono>

#include "ThreadPull.hpp"


const ThreadTask ThreadTask::ShouldDie = ThreadTask(nullptr, true );
const ThreadTask ThreadTask::NextLoop  = ThreadTask(nullptr, false);


Thread::Thread()
	: bAcive(false)
{
}

Thread::~Thread()
{
}

UNIQUE(Thread) Thread::Get()
{
	return std::make_unique<Thread>();
}

ThreadID Thread::GetThreadID()
{
	return std::this_thread::get_id();
}

void Thread::Run()
{
	std::thread([&]()
	{
		using namespace std::chrono_literals;

		const auto restTime = 5us;
		this->bAcive = true;
		
		ThreadTask currentTask = ThreadTask::NextLoop;

		while (currentTask = ThreadPull::GetRunTask(this, currentTask.task), !currentTask.bDie)
		{
			// try to take new task
			if (currentTask.task == nullptr)
			{
				std::this_thread::sleep_for(restTime); 
				continue;
			}
			
			currentTask.task->Run();
		}

		this->bAcive = false;
	}).detach();
}
