#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Traps/Wall.hpp"
#include "Traps/Teleport.hpp"

#include "Blocks/Block.hpp"
#include "Coin.hpp"
#include "Box.hpp"


class Level : public Actor
{
	GENERATED_BODY(Level, Actor);

public:
	Level()
	{
		MakeFloor({5 , 0}, {30, 0.1f});
		MakeFloor({60, 5}, {20, 0.1f});

		MakeFloor({ -40 , 0 }, { 4, 0.1f });

		MakeCatcher({0,-10}, {60, 1});

		
		MakeTeleport(FVector2(-20, 0.1f), FVector2(22,33));

		MakeWall(FVector2(-10, 0.04f));
		MakeWall(FVector2( 5 , 0.04f));
		MakeWall(FVector2( 20, 0.04f));

		MakeBlock(FVector2(30, 0.2f));
		MakeBlock(FVector2(-40, 0.2f));
	}

protected:
	
	BoxColision* MakeFloor(FVector2 location, FVector2 extents)
	{
		auto floor = CreateSubcomponent<BoxColision>("floor");
		floor->AddComponentLocation(location);
		floor->SetExtents(extents);
		floor->GetRigidBody()->SetMass(0);

		return floor;
	}

	Teleport* MakeCatcher(FVector2 location, FVector2 extents)
	{
		auto teleport = CreateActor<Teleport>("teleport", extents);
		teleport->AttachTo(this);
		teleport->SetRelativeLocation(location);
		teleport->SetTargetLocation({0, 3});

		return teleport;
	}

	Teleport* MakeTeleport(FVector2 location, FVector2 targetLocation)
	{
		auto teleport = CreateActor<Teleport>("teleport");
		teleport->AttachTo(this);
		teleport->SetRelativeLocation(location);
		teleport->SetTargetLocation(targetLocation);

		return teleport;
	}

	Wall* MakeWall(FVector2 location)
	{
		auto* wall = CreateActor<Wall>("wall");
		wall->AttachTo(this);
		wall->SetRelativeLocation(location);
		return wall;
	}

	Block* MakeBlock(FVector2 location)
	{
		auto* block = CreateActor<Block>("block");
		block->AttachTo(this);
		block->SetRelativeLocation(location);
		return block;
	}
};


#endif // !LEVEL_HPP
