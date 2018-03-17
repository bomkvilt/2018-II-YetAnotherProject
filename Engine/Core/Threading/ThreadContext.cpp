#include "ThreadContext.hpp"



std::unordered_map<ThreadID, ThreadContext> ThreadContextSingleton::contexts = std::unordered_map<ThreadID, ThreadContext>();


ThreadContext& ThreadContextSingleton::Get()
{
	return contexts[Thread::GetThreadID()];
}



Initialiser* ThreadContext::TopInitialiser()
{
	return initialiserStack.top().get();
}

void ThreadContext::PopInitialiser()
{
	initialiserStack.pop();
}
