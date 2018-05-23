#pragma once
#ifndef AVATAR_MOVEMENT_COMPONENT_HPP
#define AVATAR_MOVEMENT_COMPONENT_HPP

#include "MovementComponent.hpp"


class AvatarMovementComponent : public MovementComponent
{
	GENERATED_BODY(AvatarMovementComponent, MovementComponent);

public:
	AvatarMovementComponent();

	virtual void SetTrackingComponent(BaseActorComponent* target) override;

public:
	void StartSprinting();
	void StartWalking();
	void StartFlying();
	void Jump();

	bool IsSprinting() const;
	bool IsJumping()   const;
	bool IsWalking()   const;
	bool IsFlying()    const;

protected:
	void OnCollisionEntered(Actor*, BaseActorComponent*, FHit);
	void OnCollisionExited (Actor*, BaseActorComponent*);

private:
	size_t collidedComponents;
};


#endif // !AVATAR_MOVEMENT_COMPONENT_HPP
