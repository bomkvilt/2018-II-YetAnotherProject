#include "PGGameMode.hpp"
#include "PGPlayerController.hpp"
#include "Level.hpp"
#include "Gosha.hpp"
#include "Coin.hpp"
#include "Box.hpp"
#include "Trigger.hpp"


PGGameMode::PGGameMode()
{
	auto* level   = CreateActor<Level  >("Level"  );
	auto* coin    = CreateActor<Coin   >("Coin"   );
	auto* box1    = CreateActor<Box    >("Box"    );
	auto* box2    = CreateActor<Box    >("Box"    );
	auto* box3    = CreateActor<Box    >("Box"    );
	auto* trigger = CreateActor<Trigger>("Trigger");

	auto* gosha = CreateAvatar<Gosha >("Gosha", true);

	gosha  ->SetComponentLocation(FVector2(  0, 3 ));
	coin   ->SetComponentLocation(FVector2(  5, 1 ));
	box1   ->SetComponentLocation(FVector2( 10, 2 ));
	box2   ->SetComponentLocation(FVector2( 13, 6 ));
	box3   ->SetComponentLocation(FVector2(-15, 2 ));
	trigger->SetComponentLocation(FVector2(  6, 1 ));
}

void PGGameMode::Tick(float DeltaTime, ETickType type)
{
	Super::Tick(DeltaTime, type);
}