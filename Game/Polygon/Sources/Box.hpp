#pragma once
#ifndef BOX_HPP
#define BOX_HPP

#include "Actor.hpp"
#include "BoxColision.hpp"


class Box : public Actor
{
	GENERATED_BODY(Box, Actor);

public:
	Box()
	{
		body = CreateSubcomponent<BoxColision>("Body");
		body->SetExtents(FVector(3, 2, 0.6f));
		body->SetMass(0);
	}

protected:
	BoxColision* body;
};

#endif // !BOX_HPP