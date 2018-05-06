#include "GameMode.hpp"
#include "Threading/ThreadContext.hpp"
#include "Threading/Initialiser.hpp"
#include "PlayerController.hpp"

GameMode::GameMode()
{
	auto* init = ThreadContext::TopInitialiser();
	assert(init);
	world.reset(init->world);
	simulationMode = init->simulation;
	playerController = dynamic_cast<PlayerController*>(init->controller);
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
