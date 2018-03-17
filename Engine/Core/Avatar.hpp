#pragma once

#include "Actor.hpp"

class PlayerController;
class EventBinder;


/** Base controlable class (AI and Player)
 */
class Avatar : public Actor
{
	using Super = Actor;

public:

	Avatar();

	virtual void SetupInput(EventBinder* binder) {};

public:	//~~~~~~~~~~~~~~| 

	/// controller

	void SetPlayerController(PlayerController* newController)	{ controller = newController; }
		  PlayerController* GetPlayerController()				{ return controller; }
	const PlayerController* GetPlayerController() const			{ return controller; }

protected:

	/// >> 
	PlayerController* controller;
	/// <<

};