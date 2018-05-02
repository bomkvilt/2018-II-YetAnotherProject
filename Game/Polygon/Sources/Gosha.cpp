#include "Gosha.hpp"
#include "Coin.hpp"

Gosha::Gosha()
{
	body = CreateSubcomponent<BoxColision>("Body");
	body->SetExtents(FVector(1, 2, 0.6f));
	body->GetRigidBody()->SetMass(10);
	
	movement = CreateSubModule<MovementComponent>("Movement");
	movement->SetTrackingComponent(body);
	
	movement->states.movementMode = EMovementMode::Jumping;
	movement->states.CurrentMode().ImpactType = EMovementImpactType::Impulce;
	movement->states.CurrentMode().Impulce    = FVector2(0, 200);

	movement->states.movementMode = EMovementMode::Walk;
	movement->states.CurrentMode().ImpactType   = EMovementImpactType::Acceleration;
	movement->states.CurrentMode().Acceleration = FVector2(30, 0);

	camera = CreateSubcomponent<CameraComponent>("Camera");
	camera->AddComponentLocation(FVector(0, 0, 80), eParent);
	camera->AddComponentRotation(FQuat  (0, 90, 0), eLocal );
	camera->SetAutoregister(true);
}

void Gosha::OnBeginPlay()
{
	if (body)
	{
		body->OnCollisionEnter.Bind(this, [&](Actor* other, ActorComponent*, FHit)
		{
			if (auto* coin = dynamic_cast<Coin*>(other))
			{
				coin->AttachTo(this);
				coin->GetRootComponent()->SetMass(0);
				coin->SetRelativeLocation(FVector2(2, 0));
			}
		});
	}
}

void Gosha::SetupInput(EventBinder* binder)
{
	Super::SetupInput(binder);
	binder->BindAxis("Fwd", this, &Gosha::Move);
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
		movement->states.extraMode = EMovementMode::Jumping;
		movement->AddLenearInput(FVector2(0, 1));
	}
}
