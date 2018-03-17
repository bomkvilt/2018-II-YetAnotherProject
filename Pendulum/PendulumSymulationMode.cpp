#include "PendulumSymulationMode.hpp"

#include "Pendulum.hpp"
#include "PendulumPlayerController.hpp"


PendulumSymulationMode::PendulumSymulationMode()
{
	symlationStep = 1/120.f;

	auto* pendulum = CreateActor<Pendulum>("Pendulum", true);
}

UNIQUE(SymulationMode) PendulumSymulationMode::Get()
{
	auto* init = new Initialiser();
	init->world = MakeWorld();

	ThreadContextSingleton::Get().PushInitialiser(init);
	init->controller = MakeController();

	auto result = std::make_unique<PendulumSymulationMode>();
	ThreadContextSingleton::Get().PopInitialiser();

	return result;
}

PlayerController* PendulumSymulationMode::MakeController()
{
	return new PendulumPlayerController;
}

void PendulumSymulationMode::OnSymulationBegin()
{
	Super::OnSymulationBegin();
}

void PendulumSymulationMode::OnSymulationEnd()
{
	Super::OnSymulationEnd();
}

void PendulumSymulationMode::StopSymulation()
{
	Super::StopSymulation();
}

void PendulumSymulationMode::DoTick(float deltaTime, ETickType type)
{
	Super::DoTick(deltaTime, type);
}
