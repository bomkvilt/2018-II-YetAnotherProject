#pragma once

#include "Actor.hpp"
#include "Components/BoxColision.hpp"
#include "HealthSystem/Health.hpp"
#include "HealthSystem/Damage.hpp"
class Killer : public Actor
{
	GENERATED_BODY(Killer, Actor);

public:
	Killer()
	{
		body = CreateSubcomponent<BoxColision>("Body");
		body->SetExtents(FVector(3, 2, 0.6f));
		body->SetMass(0);
	}

	void OnBeginPlay()
	{
		if (body)
		{
			body->OnCollisionEnter.Bind(this, [&](Actor* other, BaseActorComponent*, FHit)
			{
				if (auto* target = dynamic_cast<IDamageable*>(other))
				{
					Damage damage(100);
					target->ApplyDamage(&damage);
				}
			});
		}
	}
protected:
	BoxColision * body;
};

