#include "PDGameMode.hpp"
#include "Pendulum.hpp"
#include "PDPlayerController.hpp"


PDGameMode::PDGameMode()
{
	auto* pendulum = CreateAvatar<Pendulum>("Pendulum", true);
}

void PDGameMode::Tick(float DeltaTime, ETickType type)
{
	Super::Tick(DeltaTime, type);
}