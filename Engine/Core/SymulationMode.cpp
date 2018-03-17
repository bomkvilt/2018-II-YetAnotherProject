#include "SymulationMode.hpp"


SymulationMode::SymulationMode()
	: bStop			(0)
	, symlationStep	(0)
	, lastTime		(0)
	, currentTime	(0)
{
	world.reset(ThreadContextSingleton::Get().TopInitialiser()->world);
	controller.reset(ThreadContextSingleton::Get().TopInitialiser()->controller);
}

SymulationMode::~SymulationMode()
{}

UNIQUE(SymulationMode) SymulationMode::Get()
{
	auto* init = new Initialiser;
	init->world = MakeWorld();

	ThreadContextSingleton::Get().PushInitialiser(init);
	init->controller = MakeController();
	
	auto result = std::make_unique<SymulationMode>();
	ThreadContextSingleton::Get().PopInitialiser();

	return result;
}

void SymulationMode::OnSymulationBegin()
{
	if (world) 
	{
		world->OnSymulationStart();
	}

	if (controller) 
	{
		controller->OnBeginPlay();
	}
}

void SymulationMode::OnSymulationEnd()
{
	if (world) world->OnSymulationStop();
}

void SymulationMode::DoTick(float deltaTime, ETickType type)
{
	if (type == ETickType::ePostPhysics)
	{
		controller->Tick(deltaTime, type);
	}
	if (type == ETickType::eRender)
	{
		controller->Render();
	}

	if (world) world->DoTick(deltaTime, type);
}

void SymulationMode::StopSymulation()
{
	if (world) world->OnSymulationStop();
	
	bStop = true;
}

bool SymulationMode::TickRequired() const
{
	return !bStop;
}

World* SymulationMode::MakeWorld()
{
	return new World;
}

PlayerController* SymulationMode::MakeController()
{
	return new PlayerController;
}

float SymulationMode::GetSymulationStep() 
{ 
	return symlationStep; 
}

void SymulationMode::SetDeltaTime(float delta)
{
	lastTime = currentTime;
	currentTime += delta;
}
