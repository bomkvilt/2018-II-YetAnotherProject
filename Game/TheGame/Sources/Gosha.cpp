#include "Gosha.hpp"
#include "Coin.hpp"

#include "Components/JointComponent.hpp"


Gosha::Gosha()
	: contactedBlock(nullptr)
	, attachedBlock (nullptr)
{
	body = CreateSubcomponent<BoxColision>("Body");
	body->SetExtents(FVector2(1, 2));
	body->GetRigidBody()->SetMass(10);
	
	sprite = CreateSubcomponent<SpriteComponent>("Sprite", "Hero.png");
	sprite->SetExtents(FVector2(1, 2));
	sprite->AttachTo(body);
	
	FVector epsilon(0,0,2000);
	FVector acceleration(30,0,0);
	movement->SetTrackingComponent(body);

	movement->states.movementMode = EMovementMode::Jumping;
	movement->states.ImpactType() = EMovementImpactType::Impulce;
	movement->states.NormalImpulce() = FVector2(0, 100);
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
	camera->SetRenderSize(80);

	health = CreateSubModule<HealthModule>("Health");
}

void Gosha::ApplyDamage(IDamage * damage)
{
	if (health) {
		health->ApplyDamage(damage);
		std::cout << "getdamage"<<std::endl;
	}
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
				coin->GetRootComponent()->SetMass(0);
				coin->SetRelativeLocation(FVector2(2, 0));
			}
			if (auto* block = dynamic_cast<Block*>(other))
			{
				std::cout << "Contacted" << std::endl;
				contactedBlock = block;
			}
			if (movement)
			{ // make a counter
				movement->states.movementMode = EMovementMode::Walk;
			}
		});

		body->OnCollisionExit.Bind(this, [&](Actor* other, BaseActorComponent*)
		{
			if (movement)
			{
				movement->states.movementMode = EMovementMode::falling;
			}
			if (other == contactedBlock)
			{
				std::cout << "Moved" << std::endl;
				contactedBlock = nullptr;
			}
		});
	}
}

void Gosha::SetupInput(EventBinder* binder)
{
	Super::SetupInput(binder);
	binder->BindAxis("Fwd", this, &Gosha::Move);
	binder->BindAction("Jump", BA_Pressed, this, &Gosha::Jump);
	binder->BindAction("Keep", BA_Pressed, this, &Gosha::Keep);
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

void Gosha::Keep(EKeyAction)
{
	if (attachedBlock)
	{
		DetachBlock();
	}
	else
	{
		if (contactedBlock)
		{
			AttachBlock(contactedBlock);
		}
	}
}

void Gosha::AttachBlock(Block* block)
{	
	attachedBlock = block;

	attachedBlock->AttachTo(this);
	attachedBlock->SetRelativeLocation(FVector2(0, 2.1f));
	
	std::cout << "attached " << (attachedBlock->GetRootComponent()->IsDynamic() ? "Y" : "N") << std::endl;
}

void Gosha::DetachBlock()
{
	attachedBlock->Detach();
	attachedBlock->SetComponentLocation(GetComponentLocation() + FVector2(4, -1));
	attachedBlock = nullptr;

	std::cout << "detached " << GetComponentLocation().ToString() << std::endl;
}
