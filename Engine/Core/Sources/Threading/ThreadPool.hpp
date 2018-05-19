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

	bool AddTask   (IRunable* newTask );
	bool MarkAsDone(IRunable* doneTask);
	bool HaveUnstartedTasks();
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

	std::mutex               mutex_complitted;
	std::condition_variable convar_complitted;

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

	/** Add a new task to queue
	 *	@param task	- new task
	 *	@return wether the task were added
	 */
	static bool AddTask      (IRunable*   task  );
	static bool AddTaskBacket(TaskBacket& backet);

	static ThreadTask GetRunTask(Thread* thread, IRunable* complittedTask);

	static void   SetMaxThreadCount(size_t newCount);
	static size_t GetMaxThreadCount();

protected:
	
	static ThreadTask GetRunTask_common(Thread* thread, IRunable* complittedTask);
	static ThreadTask GetRunTask_backet(Thread* thread, IRunable* complittedTask);
	
	// wether the thread should be stoped
	static bool ShouldDie(Thread* thread);

	static bool NewThreadRequired(); // mutex_threads is required
	static void CreateThread();      // mutex_threads is required
	static void DeleteThread(Thread* thread);

private:

	/// >> queued tasks
	static std::deque<IRunable*> tasks;	
	static std::mutex      mutex_tasks;
	
	static std::mutex               mutex_noTasks;
	static std::condition_variable convar_noTasks;
	/// << 

	/// >> queued taskBackets
	static std::deque<TaskBacket*> backets;
	static std::mutex        mutex_backets;
	/// <<

	/// >> created threads
	static std::atomic<bool  > bProcessBacket;
	static std::atomic<size_t> maxThreadCount;

	static std::unordered_set<Thread*> threads;
	static std::mutex            mutex_threads;
	static std::unordered_map<Thread*, UNIQUE(Thread)> allThreads;
	/// <<

};