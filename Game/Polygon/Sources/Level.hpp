#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Actor.hpp"
#include "Components/BoxColision.hpp"


class Level : public Actor
{
	GENERATED_BODY(Level, Actor);

public:
	Level()
	{
		floor = CreateSubcomponent<BoxColision>("floor");
		floor->SetExtents(FVector(15, 0.1f, 2));
		floor->GetRigidBody()->SetMass(0);
	}

protected:
	BoxColision* floor;
};


#endif // !LEVEL_HPP