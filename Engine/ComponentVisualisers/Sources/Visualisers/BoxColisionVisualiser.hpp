#pragma once
#ifndef BOX_COLLISION_VISUALISER_HPP
#define BOX_COLLISION_VISUALISER_HPP

#include "ComponentVisualiser.hpp"
#include "Components/BoxColision.hpp"

class BoxColisionVisualiser : public ComponentVisualiser
{
	virtual void Visualize(BaseActorComponent* component, ComponentVisualiserHelper& helper) override
	{
		if (auto* box = dynamic_cast<BoxColision*>(component))
		{
			FVector    extents   = box->GetExtents();
			FTransform transform = box->GetComponentTransform();
			helper.DrawShape(FShape::MakeBox(extents), transform, FColor::Green);
		}
	}
};

#endif // !BOX_COLLISION_VISUALISER_HPP