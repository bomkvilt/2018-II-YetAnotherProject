#include "ThreadPool.hpp"

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

bool TaskBacket::IsCompleted()
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
*							ThreadPool
********************************************************************/

std::deque<IRunable*>   ThreadPool::tasks			= std::deque<IRunable*>();
std::deque<IRunable*>   ThreadPool::tasks_exclusive = std::deque<IRunable*>();	
std::deque<TaskBacket*> ThreadPool::backets			= std::deque<TaskBacket*>();


std::unordered_set<Thread*> ThreadPool::threads			  = std::unordered_set<Thread*>();
std::unordered_set<Thread*> ThreadPool::threads_exclusive = std::unordered_set<Thread*>();

std::unordered_map<Thread*, UNIQUE(Thread)> ThreadPool::allThreads;


std::atomic<size_t>	ThreadPool::maxThreadCount = 1;
std::atomic<bool>   ThreadPool::bProcessBacket = false;



bool ThreadPool::AddTask(IRunable* task, bool bExlusiveThread)
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

bool ThreadPool::AddTaskBacket(TaskBacket& backet)
{
	if (backet.IsCompleted()) return true;

	{ LOCK(mutex_backets);  
		backets.emplace_back(&backet);
	}
	AddTask(new BacketRunable(), false);
	
	return true;
}

ThreadTask ThreadPool::GetRunTask(Thread* thread, IRunable* complittedTask)
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

ThreadTask ThreadPool::GetRunTask_common(Thread* thread, IRunable* complittedTask)
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

ThreadTask ThreadPool::GetRunTask_backet(Thread* thread, IRunable* complittedTask)
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
			bDone = backets.front()->IsCompleted();
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

ThreadTask ThreadPool::GetRunTask_exclusive(Thread* thread, IRunable* complittedTask)
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

bool ThreadPool::ShouldDie(Thread* thread)
{
	return threads.size() > maxThreadCount;
}

bool ThreadPool::NewThreadRequired(bool bExlusive)
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

void ThreadPool::CreateThread(bool bExlusive)
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

void ThreadPool::DeleteThread(bool bExlusive, Thread* thread)
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
