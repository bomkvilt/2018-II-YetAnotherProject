#include "ActorModule.hpp"
#include "Actor.hpp"

ActorModule::ActorModule()
	: owner(nullptr)
{
	SetOwner(ThreadContext::TopInitialiser()->owner);

	PrimatyTick.actor = owner;
}

void ActorModule::SetOwner(Actor* newOwner)
{
	UnregisterTick(PrimatyTick);

	owner = newOwner;
	PrimatyTick.actor = owner;
	RegisterTick(PrimatyTick);
}