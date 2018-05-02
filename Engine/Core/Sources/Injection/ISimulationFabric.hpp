#ifndef ISIMULATION_FABRIC_HPP
#define ISIMULATION_FABRIC_HPP

#include "Threading/Initialiser.hpp"

class PlayerController;
class SimulationMode;
class GameMode;



struct ISimulationModeFabric
{
	virtual UNIQUE(SimulationMode  ) MakeSimulationMode(SHARED(FEngineConfig) config) = 0;
	virtual UNIQUE(GameMode        ) MakeDefaultGameMode        (Initialiser* init) = 0;
	virtual UNIQUE(PlayerController) MakeDefaultPlayerController(Initialiser* init) = 0;
};





template<class _SM, class _CT, class _GM>
struct TSimulationModeFabric : public ISimulationModeFabric
{
	static UNIQUE(ISimulationModeFabric) Get()
	{
		return std::make_unique<TSimulationModeFabric<_SM, _CT, _GM>>();
	}

	virtual UNIQUE(SimulationMode) MakeSimulationMode(SHARED(FEngineConfig) config) override
	{
		return std::make_unique<_SM>(Get(), config);
	}

	virtual UNIQUE(GameMode) MakeDefaultGameMode(Initialiser* init) override
	{ 
		return ObjectCreator::CreateGameMode<_GM>(init);
	}

	virtual UNIQUE(PlayerController) MakeDefaultPlayerController(Initialiser* init) override 
	{ 
		return ObjectCreator::CreatePlayerController<_CT>(init);
	}
};

#endif // !ISIMULATION_FABRIC_HPP
