#include "BaseCollision.hpp"
#include "Injection/DependencyInjectionManager.hpp"

BaseCollision::BaseCollision(EShapeType shape)
{
	extents   = FVector(1, 1, 1);
	rigidBody = DependencyInjectionManager::MakeBody(FShape(shape, extents), this, 10);
}

void BaseCollision::SetExtents(FVector newExtents)
{
	Super::SetExtents(newExtents);

	if (rigidBody)
	{
		rigidBody->SetExtents(extents);
	}
}
