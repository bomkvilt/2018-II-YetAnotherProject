#include "Engine.hpp"

#include <thread>

namespace chrono = std::chrono;
namespace thread = std::this_thread;


Engine::Engine()
{
}

Engine::~Engine()
{
}

int Engine::MainCycle()
{
	mode->OnSymulationBegin();

	InitTime();

	size_t i = 0;
	while (mode->TickRequired())
	{
		// make check point
		STimes delta = GetDeltaTime();
		UpdateLastTime();

		// make ticks
		for (int type = 0; type < ETickType::eMAX; ++type)
		{
			mode->DoTick(delta[type].count(), (ETickType)type);
			UpdateCurrTime((ETickType)type);
		}

		// evaluate spant time
		float fstep = mode->GetSymulationStep();
	
		STime step  (fstep			);
		STime spent (GetSpentTime()	);

		//std::cout << "Tick_" << i++ << "  " << 1 / delta[0].count() << std::endl;

		// 
		if (spent < step) {
			thread::sleep_for(step - spent);
			mode->SetDeltaTime(step.count());

			UpdateCurrTime();
		}
		else {
			mode->SetDeltaTime(spent.count());
		}
	}

	return 0;
}

void Engine::SetSymulationMode(UNIQUE(SymulationMode) newMode)
{
	if (mode.get()) {
		mode->OnSymulationEnd();
	}
	mode = std::move(newMode);
}

Engine::STimes Engine::GetDeltaTime()
{
	STimes delta;

	for (int i = 0; i < delta.size(); ++i) {
		auto dt = currTime[i] - lastTime[i];
		delta[i] = dt;
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
	for (int i = 0; i < ETickType::eMAX; ++i)
		lastTime[i] = chrono::system_clock::now();

	thread::sleep_for(STime(0.001));

	for (int i = 0; i < ETickType::eMAX; ++i)
		currTime[i] = chrono::system_clock::now();
}

void Engine::UpdateLastTime()
{
	for (int i = 0; i < ETickType::eMAX; i++) {
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
		UpdateCurrTime((ETickType)i);
}
