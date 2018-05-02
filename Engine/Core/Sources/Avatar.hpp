#pragma once

#include "Actor.hpp"

class PlayerController;
class EventBinder;


/** Base controlable class (AI and Player)
 * TODO: implement movementComponent
 */
class Avatar : public Actor
{
	GENERATED_BODY(Avatar, Actor)
public:

	Avatar();

	virtual void SetupInput(EventBinder* binder) {};

public:	//~~~~~~~~~~~~~~| player controller

	void SetPlayerController(PlayerController* newController)	{ controller = newController; }
		  PlayerController* GetPlayerController()				{ return controller; }
	const PlayerController* GetPlayerController() const			{ return controller; }

protected:

	/// >> 
	PlayerController* controller;
	/// <<

};