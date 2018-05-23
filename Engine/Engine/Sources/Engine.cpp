#include "Engine.hpp"
#include "Threading/ThreadPool.hpp"
#include "Modules/ModuleManager.hpp"
#include "ComponentVisualisersModule.hpp"
#include "Reflection/Archiver.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <string>

namespace chrono = std::chrono;
namespace thread = std::this_thread;


Engine::Engine()
	: config(new FEngineConfig())
{
	ThreadPool::SetMaxThreadCount(config->countOfThreads);

	// load engine modules
	auto& manager = ModuleManager::Get();
	manager.SetConfig(config);
	manager.LoadModule<ComponentVisualisersModule>();
}

Engine::~Engine()
{
	// unload all modules
	auto& manager = ModuleManager::Get();
	manager.UnloadModules();

	// save config
	SaveConfig();
}

int Engine::MainCycle()
{
	assert(mode);

	mode->OnSimulationBegin();

	InitTime();

	while (mode->TickRequired())
	{
		// make check point
		STimes delta = GetDeltaTime();
		UpdateLastTime();

		// execute ticks
		for (int type = 0; type < ETickType::eMAX; ++type)
		{
			mode->Tick(delta[type].count(), (ETickType)type);
			UpdateCurrTime((ETickType)type);
		}

		// evaluate spant time
		float fstep = mode->GetSimulationStep();
		STime spent(GetSpentTime());
		STime step(fstep);

		// hibernate
		if (spent < step) 
		{
			thread::sleep_for(step - spent);
			mode->SetDeltaTime(step.count());

			UpdateCurrTime();
		}
		else 
		{
			mode->SetDeltaTime(spent.count());
		}
	}

	return 0;
}

void Engine::SetPathToConfig(const std::string& path)
{
	pathToConfig = path;

	std::ifstream file(path);
	if (file.is_open())
	{
		std::stringstream ss;
		ss << file.rdbuf();
		std::string data;
		data = ss.str();
		
		Archiver ar;
		ar.Constract(data);
		config->__Archive(ar, false);

		ThreadPool::SetMaxThreadCount(config->countOfThreads);
	}
}

void Engine::SetSimulationFabric(UNIQUE(ISimulationModeFabric) fabric)
{
	if (fabric)
	{
		mode = std::move(fabric->MakeSimulationMode(config));
	}
	else throw std::runtime_error("fabric must not be nulled");
}

void Engine::SaveConfig()
{
	std::ofstream file(pathToConfig);
	if (file.is_open())
	{
		Archiver ar;
		config->__Archive(ar, true);
		file << ar.Archive();
	}
}

Engine::STimes Engine::GetDeltaTime()
{
	STimes delta;

	for (int i = 0; i < delta.size(); ++i) 
	{
		delta[i] = currTime[i] - lastTime[i];
	}
	return delta;
}

Engine::STime Engine::GetSpentTime()
{
	int firstTick = 0;
	int lastTick  = ETickType::eMAX - 1;
	return currTime[lastTick] - lastTime[firstTick];
}

void Engine::InitTime()
{
	float seconds = mode->GetSimulationStep();
	auto ms    = int64_t(seconds / 1000);
	auto delta = chrono::milliseconds(ms);

	for (int i = 0; i < ETickType::eMAX; ++i)
	{
		lastTime[i] = chrono::system_clock::now();
		currTime[i] = lastTime[i] + delta;
	}
}

void Engine::UpdateLastTime()
{
	for (int i = 0; i < ETickType::eMAX; i++) 
	{
		lastTime[i] = currTime[i];
	}
}

void Engine::UpdateCurrTime(ETickType type)
{
	currTime[type] = chrono::system_clock::now();
}

void Engine::UpdateCurrTime()
{
	for (int i = 0; i < ETickType::eMAX; ++i)
	{
		UpdateCurrTime((ETickType)i);
	}
}
