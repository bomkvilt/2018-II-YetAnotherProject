#include "ThreadPool.hpp"

#include <assert.h>

#define LOCK_N(NAME, MUTEX) std::unique_lock<std::mutex> lock(MUTEX);
#define LOCK(MUTEX)	        LOCK_N(__lock__, MUTEX)

#define TRY_LOCK(MUTEX)     MUTEX.try_lock()
#define UNLOCK(MUTEX)       MUTEX.unlock()
/********************************************************************
*							TaskBacket
********************************************************************/

TaskBacket::TaskBacket()
{
	bComplitted = true;
}

bool TaskBacket::AddTask(IRunable* newTask)
{
	if (!newTask) return false;

	bComplitted = false;

	{LOCK(mutex_storedTasks);
		storedTasks.emplace_back(newTask);
	}
	return true;
}

bool TaskBacket::MarkAsDone(IRunable* doneTask)
{
	assert(doneTask);

	{LOCK(mutex_processingTasks);
		processingTasks.erase(doneTask);

		if (processingTasks.size()) return false;

		if (TRY_LOCK(mutex_storedTasks)) 
		{
			bComplitted = !storedTasks.size();
			UNLOCK(mutex_storedTasks);
			return bComplitted;
		} 
		else return false;
	}
}

bool TaskBacket::HaveUnstartedTasks()
{
	LOCK(mutex_storedTasks);
	return storedTasks.size();
}

bool TaskBacket::IsCompleted()
{
	return bComplitted;
}

IRunable* TaskBacket::GetTask()
{
	IRunable* task;
	
	{LOCK_N(lock, mutex_storedTasks);
		if (!storedTasks.size()) 
		{
			UNLOCK(lock);
			return nullptr;
		}
		task = storedTasks.front();
		storedTasks.pop_front();
		
		{LOCK(mutex_processingTasks);
			processingTasks.emplace(task);
			UNLOCK(lock);
		}
	}
	return task;
}

void TaskBacket::Wait()
{
	while (!bComplitted)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
}

/********************************************************************
*							ThreadPool
********************************************************************/

std::deque<IRunable*>       ThreadPool::tasks   = std::deque<IRunable  *>();
std::deque<TaskBacket*>     ThreadPool::backets = std::deque<TaskBacket*>();
std::unordered_set<Thread*> ThreadPool::threads = std::unordered_set<Thread*>();

std::unordered_map<Thread*, UNIQUE(Thread)> ThreadPool::allThreads;

std::mutex ThreadPool::mutex_tasks  ;
std::mutex ThreadPool::mutex_backets;
std::mutex ThreadPool::mutex_noTasks;
std::mutex ThreadPool::mutex_threads;
std::condition_variable ThreadPool::convar_noTasks;

std::atomic<size_t>	ThreadPool::maxThreadCount = 2;
std::atomic<bool>   ThreadPool::bProcessBacket = false;



bool ThreadPool::AddTask(IRunable* task)
{
	if (!task) return false;
 
	{LOCK(mutex_tasks);
		tasks.emplace_back(task);
	}
	{LOCK(mutex_threads);
		while(NewThreadRequired())
		{
			CreateThread();
		}
	}
	convar_noTasks.notify_one();

	return true;
}

bool ThreadPool::AddTaskBacket(TaskBacket& backet)
{
	if (backet.IsCompleted()) return true;

	{LOCK(mutex_backets); 
		backets.emplace_back(&backet);
	}
	AddTask(new BacketRunable());
	
	return true;
}

ThreadTask ThreadPool::GetRunTask(Thread* thread, IRunable* complittedTask)
{
	if (!thread) return ThreadTask::NextLoop;
	
	ThreadTask result = bProcessBacket
			? GetRunTask_backet(thread, complittedTask)
			: GetRunTask_common(thread, complittedTask);

	if (result == ThreadTask::NoTasksFound)
	{
		LOCK_N(lock, mutex_noTasks);
		convar_noTasks.wait(lock, [&]()
		{
			result = bProcessBacket
				? GetRunTask_backet(thread, complittedTask)
				: GetRunTask_common(thread, complittedTask);
			return result != ThreadTask::NoTasksFound;
		});
	}

	if (complittedTask)
	{
		delete complittedTask;
	}

	if (result == ThreadTask::ShouldDie)
	{
		DeleteThread(thread);
	}
	
	return result;
}

void ThreadPool::SetMaxThreadCount(size_t newCount) 
{ 
	maxThreadCount = newCount; 
}

size_t ThreadPool::GetMaxThreadCount() 
{ 
	return maxThreadCount; 
}

ThreadTask ThreadPool::GetRunTask_common(Thread* thread, IRunable* complittedTask)
{
	LOCK(mutex_tasks);

	if (ShouldDie(thread))
	{
		return ThreadTask::ShouldDie;
	}
	if (tasks.size())
	{	
		auto* task = tasks.front();
		tasks.pop_front();

		if (dynamic_cast<BacketRunable*>(task))
		{
			delete task;
			bProcessBacket = true;
			return ThreadTask::NextLoop;
		}
		else return ThreadTask(task);
	}
	else // no elements inside
	{
		return ThreadTask::NoTasksFound;
	}
}

ThreadTask ThreadPool::GetRunTask_backet(Thread* thread, IRunable* complittedTask)
{
	LOCK(mutex_backets);

	bool bDone = complittedTask
		? backets.front()->MarkAsDone(complittedTask)
		: backets.front()->IsCompleted();

	if (!bDone) // try to get a new task
	{ 
		if (IRunable* newTask = backets.front()->GetTask())
		{
			return ThreadTask(newTask);
		}
		if (backets.front()->HaveUnstartedTasks())
		{   // here we have to symulate a
			// @AddTask method to unlock
			// a weighting thread
			convar_noTasks.notify_one();
		}
		return ThreadTask::NextLoop;
	}
	else // the lates task is done
	{ 
		bProcessBacket = false;
		backets.pop_front();

		return ShouldDie(thread) 
			? ThreadTask::ShouldDie
			: ThreadTask::NextLoop;
	}
}

bool ThreadPool::ShouldDie(Thread* thread)
{ 
	LOCK(mutex_threads);
	return threads.size() > maxThreadCount;
}

bool ThreadPool::NewThreadRequired()
{
	return threads.size() < maxThreadCount;
}

void ThreadPool::CreateThread()
{
	auto    newThread  = Thread::Get();
	Thread* thread_ptr = newThread.get();

	allThreads[thread_ptr] = std::move(newThread);
	threads.emplace(thread_ptr);
	
	thread_ptr->Run();
}

void ThreadPool::DeleteThread(Thread* thread)
{
	LOCK(mutex_threads);
	threads.erase(thread);
	allThreads.erase(thread);
}
