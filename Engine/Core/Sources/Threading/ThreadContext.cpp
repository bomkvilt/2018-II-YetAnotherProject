#include "ThreadContext.hpp"



std::unordered_map<ThreadID, ThreadContext> ThreadContext::contexts = std::unordered_map<ThreadID, ThreadContext>();


ThreadContext& ThreadContext::Get()
{
	return contexts[Thread::GetThreadID()];
}

void ThreadContext::PushInitialiser(Initialiser* init)
{
	Get().initialiserStack.emplace(init);
}

Initialiser* ThreadContext::TopInitialiser()
{
	return Get().initialiserStack.top();
}

void ThreadContext::PopInitialiser()
{
	Get().initialiserStack.pop();
}
