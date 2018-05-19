#pragma once
#ifndef COIN_HPP
#define COIN_HPP

#include "Actor.hpp"
#include "Components/BoxColision.hpp"


class Coin : public Actor
{
	GENERATED_BODY(Coin, Actor);

public:
	Coin()
	{
		body = CreateSubcomponent<BoxColision>("Body");
		body->SetExtents(FVector(0.5f, 0.2f, 0.6f));
		body->GetRigidBody()->SetMass(1);
	}

protected:
	BoxColision* body;
};


#endif // !COIN_HPP
