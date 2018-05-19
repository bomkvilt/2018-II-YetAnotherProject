#ifndef ACTOR_MODULE_HPP
#define ACTOR_MODULE_HPP
#pragma once

#include "Object.hpp"


class ActorModule : public Object
{ 
	GENERATED_BODY(ActorModule, Object)

public:
	ActorModule();

public:
	/// owner
	      Actor* GetOwner()        { return owner; }
	const Actor* GetOwner() const  { return owner; }
	void SetOwner(Actor* newOwner);

protected:
	Actor* owner;
};


#endif // !ACTOR_MODULE_HPP
