#include "TGGameMode.hpp"
#include "TGPlayerController.hpp"
#include "Level.hpp"
#include "Gosha.hpp"
#include "Coin.hpp"
#include "Box.hpp"


TGGameMode::TGGameMode()
{
	auto* level = CreateActor<Level>("Level");
	auto* gosha = CreateAvatar<Gosha>("Gosha", true);

	gosha->SetComponentLocation(FVector2(0, 3));
}