#pragma once

#include <vector>
#include <stack>
#include <memory>
#include <unordered_map>

#include "../../Misc.hpp"

#include "Thread.hpp"
#include "Initialiser.hpp"

class ThreadContext;

/** Singletone containing contexts for all threads made in threadpull
 */
class ThreadContextSingleton
{
public:

	static ThreadContext& Get();

protected:

	static std::unordered_map<ThreadID, ThreadContext> contexts;
};


/**
 */
class ThreadContext
{
public:

	template<class _Init>
	void PushInitialiser(_Init* init)
	{
		this->initialiserStack.emplace(init);
	}

	Initialiser* TopInitialiser();
	
	void PopInitialiser();

protected:

	std::stack<UNIQUE(Initialiser)> initialiserStack;
};