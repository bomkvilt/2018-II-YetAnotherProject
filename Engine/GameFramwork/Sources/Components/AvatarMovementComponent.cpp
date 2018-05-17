#include "AvatarMovementComponent.hpp"

AvatarMovementComponent::AvatarMovementComponent()
	: collidedComponents(0)
{
}

void AvatarMovementComponent::SetTrackingComponent(BaseActorComponent* target)
{
	if (updatedComponent)
	{
		updatedComponent->OnCollisionEnter.Unbind(this);
		updatedComponent->OnCollisionExit .Unbind(this);
	}

	Super::SetTrackingComponent(target);

	if (updatedComponent)
	{
		updatedComponent->OnCollisionEnter.BindMethod(this, &AvatarMovementComponent::OnCollisionEntered);
		updatedComponent->OnCollisionExit .BindMethod(this, &AvatarMovementComponent::OnCollisionExited );
	}
}

void AvatarMovementComponent::StartSprinting() { states.movementMode = EMovementMode::Sprint; }
void AvatarMovementComponent::StartWalking()   { states.movementMode = EMovementMode::Walk;   }
void AvatarMovementComponent::StartFlying()    { states.movementMode = EMovementMode::flying; }

void AvatarMovementComponent::Jump()
{
	if (collidedComponents)
	{
		states.extraMode = EMovementMode::Jumping;
		AddLenearInput(FVector(0, 1, 0), eLocal);
	}
}

bool AvatarMovementComponent::IsSprinting() const { return states.CurrentModeType() == EMovementMode::Sprint;  }
bool AvatarMovementComponent::IsJumping()   const { return states.CurrentModeType() == EMovementMode::Jumping; }
bool AvatarMovementComponent::IsWalking()   const { return states.CurrentModeType() == EMovementMode::Walk;    }
bool AvatarMovementComponent::IsFlying()    const { return states.CurrentModeType() == EMovementMode::flying;  }

void AvatarMovementComponent::OnCollisionEntered(Actor*, BaseActorComponent*, FHit)
{
	if (collidedComponents++)
	{
		states.BColided() = true;
	}
}

void AvatarMovementComponent::OnCollisionExited(Actor*, BaseActorComponent*)
{
	assert(collidedComponents--);
	
	if (!collidedComponents)
	{
		states.BColided() = false;
	}
}
