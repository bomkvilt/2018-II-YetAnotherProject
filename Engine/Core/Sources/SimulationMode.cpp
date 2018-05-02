#include "SimulationMode.hpp"
#include "PlayerController.hpp"
#include "GameMode.hpp"


SimulationMode::SimulationMode(
	UNIQUE(ISimulationModeFabric) newFabric, 
	SHARED(FEngineConfig)         newConfig
)	: bStop		 (0)
	, lastTime	 (0)
	, currentTime(0)
	, fabric(std::move(newFabric))
	, config(newConfig)
{
	if (fabric && config)
	{
		auto init = Initialiser::Get();
		init->world = new World();
		init->config = config;
		init->simulation = this;

		controller = std::move(fabric->MakeDefaultPlayerController(init.get()));
		init->controller = controller.get();
		
		gameMode = std::move(fabric->MakeDefaultGameMode(init.get()) );
		if (gameMode) 
		{
			gameMode->OnConstructed();
		}
	}
	else throw std::runtime_error("fabric and config must be not nulled");
}

SimulationMode::~SimulationMode()
{
	if (gameMode)
	{
		gameMode->OnStartDestroy();
	}
}

UNIQUE(SimulationMode) SimulationMode::Get(
	UNIQUE(ISimulationModeFabric) fabric, 
	SHARED(FEngineConfig)         config
) {
	return std::make_unique<SimulationMode>(std::move(fabric), config);
}

void SimulationMode::OnSimulationBegin()
{
	if (gameMode)
	{
		gameMode->OnBeginPlay();
	}
	if (controller) 
	{
		controller->OnBeginPlay();
	}
}

void SimulationMode::OnSimulationEnd()
{
	if (gameMode)
	{
		gameMode->OnEndPlay();
	}
	if (controller)
	{
		controller->OnEndPlay();
	}
}

void SimulationMode::Tick(float deltaTime, ETickType type)
{
	if (gameMode)
	{
		gameMode->Tick(deltaTime, type);

		if (type == ETickType::ePostPhysics)
		{
			controller->Tick(deltaTime, type);
		}
		if (type == ETickType::eRender)
		{
			controller->Render();
		}
	}
}

void SimulationMode::StopSimulation()
{
	bStop = true;
}

bool SimulationMode::TickRequired() const
{
	return !bStop;
}

float SimulationMode::GetSimulationStep() 
{ 
	return config 
		? config->simulation.stepTime
		: 1/30.f;
}

void SimulationMode::SetDeltaTime(float delta)
{
	lastTime = currentTime;
	currentTime += delta;
}
