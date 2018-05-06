#pragma once

#include <chrono>
#include <array>

#include "Types.hpp"
#include "SimulationMode.hpp"
#include "Configs/EngineConfig.hpp"


/** 
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

public: //~~~~~~~~~~~~~~| interface

	int MainCycle();

	void SetPathToConfig(const std::string& path);
	void SetSimulationFabric(UNIQUE(ISimulationModeFabric) fabric);
	void SaveConfig();

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
	std::string pathToConfig;
	SHARED(FEngineConfig) config;
	UNIQUE(SimulationMode)  mode;
	/// << 

};