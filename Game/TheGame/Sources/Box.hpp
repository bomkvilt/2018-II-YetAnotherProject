#pragma once
#ifndef BOX_HPP
#define BOX_HPP

#include "Actor.hpp"
#include "Components/BoxColision.hpp"


class Box : public Actor
{
	GENERATED_BODY(Box, Actor);

public:
	
	Box()
	{
		const FVector2 wallSize(3,2);
		body = CreateSubcomponent<BoxColision>("Body");
		boxSprite = CreateSubcomponent<SpriteComponent>("WallSprite", "finish.png");
		boxSprite->SetExtents(wallSize);
		boxSprite->AttachTo(body);
		body->SetExtents(FVector(3, 2, 0.6f));
		body->SetMass(0);
	}

protected:
	BoxColision* body;
	SpriteComponent* boxSprite;
};

#endif // !BOX_HPP