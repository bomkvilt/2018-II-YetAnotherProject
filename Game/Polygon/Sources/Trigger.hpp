#pragma once

#include <iostream>

#include "Actor.hpp"
#include "ActorComponent.hpp"
#include "Components/BoxColision.hpp"


class Trigger : public Actor
{
	GENERATED_BODY(Trigger, Actor)

public:
	Trigger()
	{
		body = CreateSubcomponent<BoxColision>("Body");
		body->SetExtents(FVector(0.5f, 0.5f, 0.5f));
		body->SetMass(0);

		FCollisionRules rules;
		rules.category = 2;
		rules.rules[0] = ECollisionMode::eOverlap;
		body->GetRigidBody()->SetCollisionRules(rules);
	}

	virtual void OnBeginPlay() override
	{
		body->OnCollisionEnter.Bind(this, [&](Actor*, BaseActorComponent*, FHit)
		{
			std::cout << "triggered" << std::endl;
		});

		body->OnCollisionExit.Bind(this, [&](Actor*, BaseActorComponent*)
		{
			std::cout << "untriggered" << std::endl;
		});
	}

protected:
	BoxColision* body;
};