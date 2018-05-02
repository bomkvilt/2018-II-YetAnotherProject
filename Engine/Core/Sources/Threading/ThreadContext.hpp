#ifndef THREAD_CONTEXT_HPP
#define THREAD_CONTEXT_HPP

#include <stack>
#include <memory>
#include <unordered_map>

#include "Misc.hpp"

#include "Thread.hpp"
#include "Initialiser.hpp"


/** TODO: make the class' constructor private
 *	NOTE: when it's done there are an acces error of inner classes
 */
class ThreadContext
{
private:
	using Contexts = std::unordered_map<ThreadID, ThreadContext>;

protected:
	std::stack<Initialiser*> initialiserStack;

public:
	static void PushInitialiser(Initialiser* init);
	static Initialiser* TopInitialiser();
	static void			PopInitialiser();

public:
	class ScopeInitHelper final
	{
	public:
		explicit ScopeInitHelper(Initialiser* init)
		{
			ThreadContext::PushInitialiser(init);
		}
		~ScopeInitHelper()
		{
			ThreadContext::PopInitialiser();
		}
	};

	static inline ScopeInitHelper ScopeInit(Initialiser* init)
	{
		return ScopeInitHelper(init);
	}

protected:

	static ThreadContext& Get();

	static Contexts contexts;
};

#define SCOPE_INIT(INIT) auto __SCOPE_INIT = ThreadContext::ScopeInit(INIT)

#endif // !THREAD_CONTEXT_HPP