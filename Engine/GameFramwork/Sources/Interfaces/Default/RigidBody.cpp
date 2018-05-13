#include "RigidBody.hpp"
#include "BaseActorComponent.hpp"


FRigidBody::FRigidBody(BaseActorComponent* owner)
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
