#pragma once

#include <chrono>

#include "World.hpp"
#include "SimulationMode.hpp"

/**
 * 
 */
class Engine
{
	using Time  = std::chrono::system_clock::time_point;
	using STime = std::chrono::duration<float>;

	using Times  = std::array<Time,  ETickType::eMAX>;
	using STimes = std::array<STime, ETickType::eMAX>;

public:

	Engine();
	~Engine();

public: //~~~~~~~~~~~~~~| Main cycle

	int MainCycle();

public: //~~~~~~~~~~~~~~| Mode

	void SetSimulationMode(UNIQUE(SimulationMode) newMode);

protected:

	STimes GetDeltaTime();
	STime  GetSpentTime();

	void InitTime();
	void UpdateLastTime();
	void UpdateCurrTime(ETickType type);
	void UpdateCurrTime();

protected:

	/// >> time
	Times lastTime;
	Times currTime;
	/// << 


	/// >> mode
	UNIQUE(SimulationMode) mode;
	/// << 

};