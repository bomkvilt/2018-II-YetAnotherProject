#pragma once
#ifndef AVATAR_HPP
#define AVATAR_HPP

#include "Actor.hpp"
#include "Components/AvatarMovementComponent.hpp"

class PlayerController;
class EventBinder;


class Avatar : public Actor
{
	GENERATED_BODY(Avatar, Actor)
public:

	Avatar();

	virtual void SetupInput(EventBinder* binder) {};

public:	//~~~~~~~~~~~~~~| player controller

	      PlayerController* GetPlayerController()       { return controller; }
	const PlayerController* GetPlayerController() const	{ return controller; }

	      AvatarMovementComponent* GetMovement()       { return movement; }
	const AvatarMovementComponent* GetMovement() const { return movement; }

protected:

	PlayerController* controller;
	AvatarMovementComponent* movement;
};


#endif //!AVATAR_HPP