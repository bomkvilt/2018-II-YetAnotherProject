#include "ObjectBase.hpp"

ObjectBase::ObjectBase()
	: world(ThreadContextSingleton::Get().TopInitialiser()->world)
	, name (*ThreadContextSingleton::Get().TopInitialiser()->name)
	, ouid (ThreadContextSingleton::Get().TopInitialiser()->ouid )
{}

ObjectBase::~ObjectBase()
{
}
