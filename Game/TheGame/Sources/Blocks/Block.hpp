#pragma once

#include "Actor.hpp"
#include "Components/BoxColision.hpp"


class Block : public Actor
{
	GENERATED_BODY(Block, Actor)

public:

	Block()
	{
		body = CreateSubcomponent<BoxColision>("body");
		body->SetExtents(FVector2(3, 0.1f));
		body->SetMass(0);
	}

public:
	BoxColision* body;
};