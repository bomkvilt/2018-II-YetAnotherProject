#include "Object.hpp"

#include "World.hpp"

Object::Object()
{
	PrimatyTick.tickType = ETickType::ePostPhysics;
	PrimatyTick.BindFuctoin(&Object::Tick, this);
}

void Object::OnConstructed()
{
	RegisterTick(PrimatyTick);
}

void Object::OnBeginPlay()
{
}

void Object::OnEndPlay()
{
	UnregisterTick(PrimatyTick);
}

void Object::OnStartDestroy()
{
}

bool Object::IsValid(Object * object) const
{
	return world && world->IsValid(object);
}

void Object::Tick(float DeltaTime, ETickType type) 
{
}

void Object::RegisterTick(ITickFunction& tick)
{
	if (!world) return;

	world->RegisterTickFunction(tick);
}

void Object::UnregisterTick(ITickFunction& tick)
{
	if (!world) return;

	world->UnregisterTickFunction(tick);
}
