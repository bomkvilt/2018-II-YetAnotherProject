#pragma once

#include <Actor.hpp>

#include "Components/BoxColision.hpp"

#include "Gosha.hpp"


class Teleport : public Actor
{
	GENERATED_BODY(Teleport, Actor)

public:
	Teleport(FVector2 extents = FVector2(1, 0.1f))
	{
		trigger = CreateSubcomponent<BoxColision>("Trigger");
		trigger->SetExtents(extents);
		trigger->SetMass(0);
	}

	void SetTargetLocation(const FVector2& newTarget)
	{
		targetLocation = newTarget;
	}

	virtual void OnBeginPlay() override
	{
		trigger->OnCollisionEnter.Bind(this, [&](Actor* other, BaseActorComponent*, FHit)
		{
			if (auto* gosha = dynamic_cast<Gosha*>(other))
			{
				gosha->SetComponentLocation(targetLocation);
			}
		});
	}

private:
	FVector2 targetLocation;
	BoxColision* trigger;
};