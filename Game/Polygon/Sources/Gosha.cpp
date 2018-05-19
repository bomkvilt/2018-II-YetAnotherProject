#include "Gosha.hpp"
#include "Coin.hpp"

#include "Threading/ObjectLocker.hpp"
#include "Components/JointComponent.hpp"


Gosha::Gosha()
{
	body = CreateSubcomponent<BoxColision>("Body");
	body->SetExtents(FVector(1, 2, 0.6f));
	body->GetRigidBody()->SetMass(10);
	
	
	FVector epsilon(0,0,2000);
	FVector acceleration(30,0,0);
	movement->SetTrackingComponent(body);

	movement->states.movementMode = EMovementMode::Jumping;
	movement->states.ImpactType() = EMovementImpactType::Impulce;
	movement->states.NormalImpulce() = FVector2(0, 200);
	movement->states.NormalEpsilon() = epsilon;

	movement->states.movementMode = EMovementMode::falling;
	movement->states.ImpactType() = EMovementImpactType::Acceleration;
	movement->states.NormalAcceleration() = acceleration;
	movement->states.NormalEpsilon() = epsilon;

	movement->states.movementMode = EMovementMode::Walk;
	movement->states.ImpactType() = EMovementImpactType::Acceleration;
	movement->states.NormalAcceleration() = acceleration;
	movement->states.NormalEpsilon() = epsilon;

	camera = CreateSubcomponent<CameraComponent>("Camera");
	camera->AddComponentLocation(FVector(0, 0, 80), eParent);
	camera->AddComponentRotation(FQuat  (0, 90, 0), eLocal );
	camera->SetAutoregister(true);
}

void Gosha::OnBeginPlay()
{
	if (body)
	{
		body->OnCollisionEnter.Bind(this, [&](Actor* other, BaseActorComponent*, FHit)
		{
			if (auto* coin = dynamic_cast<Coin*>(other))
			{
				coin->AttachTo(this);
				coin->GetRootComponent()->GetRigidBody()->SetBodyType(ERigidBodyType::eIgnore);
				coin->SetRelativeLocation(FVector2(0, 2.5f));
				return;
			}
			if (movement)
			{ // make a counter
				movement->states.movementMode = EMovementMode::Walk;
				return;
			}
		});

		body->OnCollisionExit.Bind(this, [&](Actor* other, BaseActorComponent*)
		{
			if (movement)
			{
				movement->states.movementMode = EMovementMode::falling;
				return;
			}
		});
	}
}

void Gosha::SetupInput(EventBinder* binder)
{
	Super::SetupInput(binder);
	binder->BindAxis("Fwd", this, &Gosha::Move);
	binder->BindAxis("R"  , this, [&](auto, float value){ camera->AddComponentRotation(FQuat(0, 5 * value, 0), eParent); });
	binder->BindAction("Jump", BA_Pressed, this, &Gosha::Jump);
}

void Gosha::Move(float value)
{
	if (movement && value)
	{
		movement->AddLenearInput(FVector2(value, 0));
	}
}

void Gosha::Jump(EKeyAction)
{
	if (movement)
	{
		movement->Jump();
	}
}
