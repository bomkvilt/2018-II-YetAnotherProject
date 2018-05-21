#pragma once

#include "Actor.hpp"
#include "Components/BoxColision.hpp"
#include "Components/JointComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Presets/CollisionPresets.hpp"
#include "Gosha.hpp"


class Wall : public Actor
{
	GENERATED_BODY(Wall, Actor)

public:
	Wall()
		: bTriggered(false)
	{
		const FVector2 triggerSize(  3 , 2);
		const FVector2 wallSize   (  1 , 4);
		const FVector2 borderSize (0.1f, 4);

		const FVector2 borderOffset1(wallSize.X + borderSize.X + 0.05, 9);
		const FVector2 borderOffset2(borderOffset1 * FVector2(-1, 1));
		const FVector2 triggerOffset(0, triggerSize.Y );
		const FVector2 wallOffset   (0, wallSize   .Y );

		base = CreateSubcomponent<BoxColision>("base");
		base->SetCollisionRules(CollisionPresets::Get()[eCT_Ignore]);
		base->SetExtents(FVector2(1, 0.01f));
		base->SetMass(0);

		trigger = CreateSubcomponent<BoxColision>("Trigger");
		trigger->SetCollisionRules(CollisionPresets::Get()[eCT_Trigger]);
		trigger->SetRelativeLocation(triggerOffset);
		trigger->SetExtents         (triggerSize  );
		trigger->SetMass(0);

		joint = CreateSubcomponent<JointComponent>("Joint");
		joint->SetConstraint(FConstraintType::MakeAxis(eY, 0, 9));
		joint->AttachTo(base);

		// wall
		wall = CreateSubcomponent<BoxColision>("Wall");
		wall->SetRelativeLocation(wallOffset);
		wall->SetExtents         (wallSize  );
		wall->SetMass(1500000);
		wall->AttachTo(joint);

		wallSprite = CreateSubcomponent<SpriteComponent>("WallSprite", "Wall.jpg");
		wallSprite->SetExtents(wallSize);
		wallSprite->AttachTo(wall);

		// border 1
		border1 = CreateSubcomponent<BoxColision>("border1");
		border1->SetRelativeLocation(borderOffset1);
		border1->SetExtents         (borderSize   );
		border1->SetMass(0);

		borderSprite1 = CreateSubcomponent<SpriteComponent>("BorderSprite1", "WallBorder.jpg");
		borderSprite1->SetExtents(borderSize);
		borderSprite1->AttachTo(border1);

		// border 2
		border2 = CreateSubcomponent<BoxColision>("border2");
		border2->SetRelativeLocation(borderOffset2);
		border2->SetExtents         (borderSize   );
		border2->SetMass(0);

		borderSprite2 = CreateSubcomponent<SpriteComponent>("BorderSprite2", "WallBorder.jpg");
		borderSprite2->SetExtents(borderSize);
		borderSprite2->AttachTo(border2);
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

	SpriteComponent* wallSprite;
	SpriteComponent* borderSprite1;
	SpriteComponent* borderSprite2;

	BoxColision* base;
	BoxColision* wall;
	BoxColision* border1;
	BoxColision* border2;
	BoxColision* trigger;
	JointComponent* joint;
};