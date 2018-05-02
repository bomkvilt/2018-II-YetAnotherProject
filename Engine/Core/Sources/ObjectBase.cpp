#include "ObjectBase.hpp"

ObjectBase::ObjectBase()
{
	auto* init = ThreadContext::TopInitialiser();
	assert(init && init->name);
	world =  init->world;
	name  = *init->name;
	ouid  =  init->ouid;
	type  =  init->type;
}

ObjectBase::~ObjectBase()
{
}

void ObjectBase::OnConstructed()
{
}

void ObjectBase::OnBeginPlay()
{
}

void ObjectBase::OnEndPlay()
{
}

void ObjectBase::OnStartDestroy()
{
}


