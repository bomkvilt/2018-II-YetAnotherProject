#include "RigidBody.hpp"
#include "ActorComponent.hpp"


FRigidBody::FRigidBody(ActorComponent* owner)
	: owner(owner)
{}

World * FRigidBody::GetWorld()
{
	if (owner)
	{
		return owner->GetWorld();
	}
	return nullptr;
}
