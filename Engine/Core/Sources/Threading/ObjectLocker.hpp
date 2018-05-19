#pragma once
#ifndef OBJECT_LOCKER_HPP
#define OBJECT_LOCKER_HPP

#include "Actor.hpp"


struct ObjectLocker
{
	ObjectLocker(Actor* target, Actor* self = nullptr)
		: self  (self  )
		, target(target)
	{
		if (!target) return;

		if (self)
		{
			self->lock.unlock();

			std::lock(self->lock, target->lock);
		}
		else
		{
			target->lock.lock();
		}
	}

	~ObjectLocker()
	{
		if (target)
		{
			target->lock.unlock();
		}
	}


private:
	Actor* target;
	Actor* self;
};


#endif // !OBJECT_LOCKER_HPP
