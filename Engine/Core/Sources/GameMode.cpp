#include "GameMode.hpp"
#include "Threading/ThreadContext.hpp"
#include "Threading/Initialiser.hpp"
#include "BasePlayerController.hpp"

GameMode::GameMode()
{
	if (auto* init = ThreadContext::TopInitialiser())
	{
		world.reset(init->world);
		simulationMode   = init->simulation;
		playerController = init->controller;
	}
}

void GameMode::OnBeginPlay()
{
	if (world) world->OnSimulationStart();
}

void GameMode::OnEndPlay()
{
	if (world) world->OnSimulationStop();
}

void GameMode::Tick(float DeltaTime, ETickType type)
{
	if (world)
	{
		world->DoTick(DeltaTime, type);
	}
}
