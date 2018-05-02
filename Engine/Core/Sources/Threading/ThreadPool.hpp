#pragma once

#include <vector>
#include <atomic>
#include <stack>
#include <deque>
#include <mutex>
#include <unordered_set>
#include <unordered_map>

#include "Runable.hpp"
#include "Thread.hpp"


class TaskBacket
{
public:

	 TaskBacket();
	~TaskBacket();


	bool AddTask   (IRunable* newTask);
	bool MarkAsDone(IRunable* doneTask);
	bool IsCompleted();
	
	IRunable* GetTask();

	void Wait();

protected:
	/// >> 
	std::deque<IRunable*> storedTasks;
	std::mutex		mutex_storedTasks;
	/// <<

	/// >>
	std::unordered_set<IRunable*> processingTasks;
	std::mutex				mutex_processingTasks;
	/// <<

	std::atomic<bool> bComplitted;
};


/** Sungletone handleing threads and takeing Ranables
 */
class ThreadPool
{
	struct BacketRunable : public IRunable
	{
		virtual void Run() override {};
	};

public:

	/** Add new task to queue
	 *	@param task				- new task
	 *	@param bExlusiveThread	- if true create a new thread and load the task on the one
	 *								NOTE: the thread will not depend from @maxThreadCount
	 *	@return wether the task were added
	 */
	static bool AddTask(IRunable* task, bool bExclusiveThread);
	static bool AddTaskBacket(TaskBacket& backet);

	static ThreadTask GetRunTask(Thread* thread, IRunable* complittedTask);

	static void   SetMaxThreadCount(size_t newCount)	{ maxThreadCount = newCount; }
	static size_t GetMaxThreadCount()					{ return maxThreadCount;	 }

protected:

	static ThreadTask GetRunTask_common   (Thread* thread, IRunable* complittedTask);
	static ThreadTask GetRunTask_backet   (Thread* thread, IRunable* complittedTask);
	static ThreadTask GetRunTask_exclusive(Thread* thread, IRunable* complittedTask);

	static bool ShouldDie		 (Thread* thread);
	static bool NewThreadRequired(bool bExlusive);

	//NOTE: unsafe
	static void CreateThread(bool bExlusive);
	static void DeleteThread(bool bExlusive, Thread* thread);

private:

	/// >> queued tasks
	static std::deque<IRunable*> tasks;	
	static std::mutex	   mutex_tasks;

	static std::deque<IRunable*> tasks_exclusive;	
	static std::mutex	   mutex_tasks_exclusive;
	/// << 

	/// >> queued taskBackets
	static std::deque<TaskBacket*> backets;
	static std::mutex		 mutex_backets;
	/// <<

	/// >> created threads
	static std::atomic<size_t> maxThreadCount;

	static std::unordered_set<Thread*> threads;
	static std::mutex			 mutex_threads;

	static std::unordered_set<Thread*> threads_exclusive;
	static std::mutex			 mutex_threads_exclusive;
	
	static std::atomic<bool> bProcessBacket;

	static std::unordered_map<Thread*, UNIQUE(Thread)> allThreads;
	/// <<

};