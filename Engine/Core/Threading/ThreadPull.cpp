#include "ThreadPull.hpp"

#include <assert.h>

#define LOCK(MUTEX) std::unique_lock<std::mutex> lock

/********************************************************************
*							TaskBacket
********************************************************************/

TaskBacket::TaskBacket()
{
	bComplitted = true;
}

TaskBacket::~TaskBacket()
{
}

bool TaskBacket::AddTask(IRunable* newTask)
{
	if (!newTask) return false;

	bComplitted = false;

	{ LOCK(mutex_storedTasks);
		storedTasks.emplace_back(newTask);
	}
	return true;
}

bool TaskBacket::MarkAsDone(IRunable* doneTask)
{
	assert(doneTask);

	{ LOCK(mutex_processingTasks);
		processingTasks.erase(doneTask);

		if (processingTasks.size()) return false;

		if (mutex_storedTasks.try_lock()) 
		{
			bool bDone = !storedTasks.size();

			mutex_storedTasks.unlock();

			bComplitted = bDone;

			return bDone;
		} 
		else return false;
	}
}

bool TaskBacket::IsComplitted()
{
	return bComplitted;
}

IRunable* TaskBacket::GetTask()
{
	IRunable* task;
	
	{ mutex_storedTasks.lock();
		if (!storedTasks.size()) 
		{
			mutex_storedTasks.unlock();
			return nullptr;
		}
		task = storedTasks.front();
		storedTasks.pop_front();
		
		{ LOCK(mutex_processingTasks);
			processingTasks.emplace(task);
			mutex_storedTasks.unlock();
		}
	}
	return task;
}

void TaskBacket::Wait()
{
	while (!bComplitted)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(300));
	}
}

/********************************************************************
*							ThreadPull
********************************************************************/

std::deque<IRunable*>   ThreadPull::tasks			= std::deque<IRunable*>();
std::deque<IRunable*>   ThreadPull::tasks_exclusive = std::deque<IRunable*>();	
std::deque<TaskBacket*> ThreadPull::backets			= std::deque<TaskBacket*>();


std::unordered_set<Thread*> ThreadPull::threads			  = std::unordered_set<Thread*>();
std::unordered_set<Thread*> ThreadPull::threads_exclusive = std::unordered_set<Thread*>();

std::unordered_map<Thread*, UNIQUE(Thread)> ThreadPull::allThreads;


std::atomic<size_t>	ThreadPull::maxThreadCount = 1;	//TODO: rocever the multythreading (if there are more then 1 thread - boom | when you try to take a new task)
std::atomic<bool>   ThreadPull::bProcessBacket = false;



bool ThreadPull::AddTask(IRunable* task, bool bExlusiveThread)
{
	if (!task) return false;

	if (!bExlusiveThread) 
	{ 
		{ LOCK(mutex_tasks);
			tasks.emplace_back(task);
		}
		{ LOCK(mutex_threads);
			if (NewThreadRequired(bExlusiveThread))
			{
				CreateThread(bExlusiveThread);
			}
		}
	}
	else
	{
		{ LOCK(mutex_tasks_exclusive);
			tasks_exclusive.emplace_back(task);
		}
		{ LOCK(mutex_threads_exclusive);
			if (NewThreadRequired(bExlusiveThread))
			{
				CreateThread(bExlusiveThread);
			}
		}
	}
	return true;
}

bool ThreadPull::AddTaskBacket(TaskBacket& backet)
{
	if (backet.IsComplitted()) return true;

	{ LOCK(mutex_backets);  
		backets.emplace_back(&backet);
	}
	AddTask(new BacketRunable(), false);
	
	return true;
}

ThreadTask ThreadPull::GetRunTask(Thread* thread, IRunable* complittedTask)
{
	if (!thread) return ThreadTask();

	bool bExclusive = false;
	ThreadTask result;

	{ LOCK(mutex_threads_exclusive);
		if (threads_exclusive.count(thread))
		{
			result = GetRunTask_exclusive(thread, complittedTask);
			bExclusive = true;
		}
	}

	if (!bExclusive)
	{
		if (bProcessBacket)
		{
			result = GetRunTask_backet(thread, complittedTask);
		}
		else
		{
			result = GetRunTask_common(thread, complittedTask);
		}
	}

	if (complittedTask)
	{
		delete complittedTask;
	}

	if (result == ThreadTask::ShouldDie)
	{
		DeleteThread(bExclusive, thread);
	}

	return result;
}

ThreadTask ThreadPull::GetRunTask_common(Thread* thread, IRunable* complittedTask)
{
	ThreadTask result = ThreadTask::NextLoop;

	{ LOCK(mutex_tasks);
		if (ShouldDie(thread))
		{
			result = ThreadTask::ShouldDie;
		}
		else if (tasks.size())
		{	// get new task
			result.task = tasks.front();
			result.bDie = false;
			tasks.pop_front();

			// start process a backet
			if (dynamic_cast<BacketRunable*>(result.task))
			{
				delete result.task;
				bProcessBacket = true;

				result = ThreadTask::NextLoop;
			}
		}
		else 
		{
			result = ThreadTask::NextLoop;
		}
	}

	return result;
}

ThreadTask ThreadPull::GetRunTask_backet(Thread* thread, IRunable* complittedTask)
{
	ThreadTask result = ThreadTask::NextLoop;

	{ LOCK(mutex_backets);
		bool bDone = false;
		if (complittedTask)
		{ 
			bDone = backets.front()->MarkAsDone(complittedTask);
		}
		else
		{
			bDone = backets.front()->IsComplitted();
		}

		if (!bDone) // try to get a new task
		{ 
			IRunable* newTask = backets.front()->GetTask();
			if (newTask)
			{
				result.task = newTask;
				result.bDie = false;
			}
			else 
			{
				result = ThreadTask::NextLoop;
			}
		}

		if (bDone) // the lates task is done
		{ 
			bProcessBacket = false;
			backets.pop_front();

			result = ShouldDie(thread)
				? ThreadTask::ShouldDie
				: ThreadTask::NextLoop;
		}
	}

	return result;
}

ThreadTask ThreadPull::GetRunTask_exclusive(Thread* thread, IRunable* complittedTask)
{
	ThreadTask result = ThreadTask::NextLoop;

	{ LOCK(mutex_tasks_exclusive);
		if (tasks_exclusive.size())
		{ 
			result.task = tasks_exclusive.front();
			result.bDie = false;
			tasks_exclusive.pop_front();

			return result;
		}
	}

	{ LOCK(mutex_threads);
		threads_exclusive.erase(thread);
		threads.emplace(thread);
		return result;
	}
}

bool ThreadPull::ShouldDie(Thread* thread)
{
	return threads.size() > maxThreadCount;
}

bool ThreadPull::NewThreadRequired(bool bExlusive)
{
	if (!bExlusive)
	{
		return threads.size() < maxThreadCount;
	}
	else
	{
		return true;
	}
}

void ThreadPull::CreateThread(bool bExlusive)
{
	UNIQUE(Thread) newThread = Thread::Get();
	
	Thread* thread_ptr = newThread.get();

	allThreads[thread_ptr] = std::move(newThread);

	if (!bExlusive)
	{
		threads.emplace(thread_ptr);
	}
	else
	{
		threads_exclusive.emplace(thread_ptr);
	}

	thread_ptr->Run();
}

void ThreadPull::DeleteThread(bool bExlusive, Thread* thread)
{
	if (!bExlusive)
	{
		threads.erase(thread);
	}
	else
	{
		threads_exclusive.erase(thread);
	}
	allThreads.erase(thread);
}
