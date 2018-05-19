#include "BaseCollision.hpp"
#include "Injection/DependencyInjectionManager.hpp"

BaseCollision::BaseCollision(EShapeType shape)
	: extents(1, 1, 1)
{
	rigidBody = DependencyInjectionManager::MakeBody(FShape(shape, extents), this, 10);
}

FVector BaseCollision::GetExtents() const
{
	return extents;
}

void BaseCollision::SetExtents(FVector newExtents)
{
	extents = newExtents;
	if (rigidBody)
	{
		rigidBody->SetExtents(extents);
	}
}
