#include "Thread.hpp"

#include <chrono>
#include <iostream>
#include "ThreadPool.hpp"

const ThreadTask ThreadTask::NoTasksFound = ThreadTask(nullptr, ThreadTask::eNoTasksFound);
const ThreadTask ThreadTask::ShouldDie    = ThreadTask(nullptr, ThreadTask::eShouldDie   );
const ThreadTask ThreadTask::NextLoop     = ThreadTask(nullptr, ThreadTask::eNextLoop    );


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
		ThreadTask currentTask = ThreadTask::NextLoop;

		while ((currentTask = ThreadPool::GetRunTask(this, currentTask.task)) != ThreadTask::ShouldDie)
		{
			// try to take a new task
			if (currentTask.task == nullptr)
			{
				std::this_thread::sleep_for(5us); 
				continue;
			}
			currentTask.task->Run();
		}
	}).detach();
}
