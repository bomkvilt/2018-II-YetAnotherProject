#ifndef SIMULATION_MODE_HPP
#define SIMULATION_MODE_HPP
#pragma once

#include "Common.hpp"

#include "Configs/EngineConfig.hpp"
#include "Injection/ISimulationFabric.hpp"

class GameMode;
class PlayerController;


/** A class handling a symualtion
 *	Tasks:
 *	. world initialisation
 *	. 
 */
class SimulationMode
{
public:

	SimulationMode(
		UNIQUE(ISimulationModeFabric) fabric, 
		SHARED(FEngineConfig)         config
		);
	virtual ~SimulationMode();

	static UNIQUE(SimulationMode) Get(
		UNIQUE(ISimulationModeFabric) fabric, 
		SHARED(FEngineConfig)         config
		);

public: //~~~~~~~~~~~~~~| API for an @Engine

	virtual void OnSimulationBegin();
	virtual void OnSimulationEnd();

	virtual void Tick(float deltaTime, ETickType type);
	
	virtual void StopSimulation();
	virtual bool TickRequired() const;

public:

	float GetSimulationStep();

	void SetDeltaTime(float delta);

protected:

	/// >> tick
	bool bStop;
	/// << 

	/// >> time (seconds)
	float currentTime;
	float lastTime;
	/// <<

	/// >>
	SHARED(FEngineConfig)         config;
	UNIQUE(ISimulationModeFabric) fabric;
	UNIQUE(PlayerController) controller;
	UNIQUE(GameMode) gameMode;
	/// <<
};

#endif // SIMULATION_MODE_HPP