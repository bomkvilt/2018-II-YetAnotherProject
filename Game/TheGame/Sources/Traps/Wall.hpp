#pragma once

#include "Actor.hpp"
#include "Components/BoxColision.hpp"
#include "Gosha.hpp"


class Wall : public Actor
{
	GENERATED_BODY(Wall, Actor)

public:
	Wall()
		: bTriggered(false)
	{
		trigger = CreateSubcomponent<BoxColision>("Trigger");
		trigger->SetExtents(FVector2(3, 0.08f));
		trigger->SetMass(0);

		wall = CreateSubcomponent<BoxColision>("Wall");
		wall->SetRelativeLocation(FVector2(0, 2.2f));
		wall->SetExtents(FVector2(1, 4));
		wall->SetMass(1500000);

		border1 = CreateSubcomponent<BoxColision>("border1");
		border1->SetRelativeLocation(FVector2( 1.15f, 11));
		border1->SetExtents(FVector2(0.1f, 4));
		border1->SetMass(0);

		border2 = CreateSubcomponent<BoxColision>("border2");
		border2->SetRelativeLocation(FVector2(-1.15f, 11));
		border2->SetExtents(FVector2(0.1f, 4));
		border2->SetMass(0);
	}

	void SetTargetLocation(const FVector2& newTarget)
	{
		targetLocation = newTarget;
	}

	virtual void Tick(float DeltaTime, ETickType type)
	{
		if (bTriggered)
		{
			wall->AddForce(FVector2(0, 600), eWorld);
		}

		if (wall->GetRelativeLocation().Y >= 12)
		{
			wall->SetRelativeLocation(FVector2(0, 12));
		}
	}

	virtual void OnBeginPlay() override
	{
		trigger->OnCollisionEnter.Bind(this, [&](Actor* other, BaseActorComponent*, FHit)
		{
			if (auto* gosha = dynamic_cast<Gosha*>(other))
			{
				bTriggered = true;
			}
		});

		trigger->OnCollisionExit.Bind(this, [&](Actor* other, BaseActorComponent*)
		{
			if (auto* gosha = dynamic_cast<Gosha*>(other))
			{
				bTriggered = false;
			}
		});
	}

private:
	bool bTriggered;

	FVector2 targetLocation;
	BoxColision* wall;
	BoxColision* border1;
	BoxColision* border2;
	BoxColision* trigger;
};