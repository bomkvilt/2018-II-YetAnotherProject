#ifndef DELEGATE_HPP
#define DELEGATE_HPP


#include <vector>
#include <functional>
#include <algorithm>

#include "World.hpp"

class Object;


template<typename... _Ts>
class Delegate
{
public:
	Delegate(World* world)
		: world(world)
	{}

	template<class _T, typename _Fn>
	void Bind(_T* target, _Fn fn)
	{
		if (target)
		{
			slots.emplace_back(target, fn);
		}
	}

	template<class _T, typename _Fn>
	void BindMethod(_T* target, _Fn fn)
	{
		if (target)
		{
			slots.emplace_back(target, [target, fn](_Ts... args)
			{
				(target->*fn)(args...);
			});
		}
	}

	template<class _T>
	void Unbind(_T* target)
	{
		if (!target) return;
		
		auto itr = slots.begin();
		auto end = slots.end();
		while (itr != end)
		{
			if (itr->target != target) 
			{
				++itr;
			}
			else 
			{
				itr = slots.erase(itr);
				end = slots.end();
			}
		}
	}

	void Broadcast(_Ts... args)
	{
		auto itr = slots.begin();
		auto end = slots.end();
		while (itr != end)
		{
			if (IsValid(*itr))
			{
				(*itr++)(args...);
			}
			else
			{
				itr = slots.erase(itr);
				end = slots.end();
			}
		}
	}

private:
	struct Slot
	{
		using Callback = std::function<void(_Ts...)>;
		
		Object*  target;
		Callback callback;
	
	public: 
		Slot(Object* target, Callback callback) 
			: callback(callback)
			, target  (target) 
		{}

		void operator()(_Ts... args) const
		{
			callback(args...);
		}
	};

private:
	bool IsValid(Slot& slot) const
	{
		return world && world->IsValid(slot.target);
	}

	World*            world;
	std::vector<Slot> slots;
};



#endif // !DELEGATE_HPP
