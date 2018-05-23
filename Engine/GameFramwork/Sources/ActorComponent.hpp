#pragma once
#ifndef ACTOR_COMPONENT_HPP
#define ACTOR_COMPONENT_HPP

#include "BaseActorComponent.hpp"


class PlayerController;


class ActorComponent : public BaseActorComponent
{
	GENERATED_BODY(ActorComponent, BaseActorComponent)

public:
	      PlayerController* GetPlayerController();
	const PlayerController* GetPlayerController() const;
};



#endif // !ACTOR_COMPONENT_HPP
