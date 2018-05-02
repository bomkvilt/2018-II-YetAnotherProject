#ifndef GAME_MODE_HPP
#define GAME_MODE_HPP

#include <memory>

#include "Misc.hpp"
#include "ObjectCreator.hpp"
#include "World.hpp"

class SimulationMode;


class GameMode
{
public:

	GameMode();
	
	virtual void OnConstructed()  {}
	virtual void OnBeginPlay()	  {}
	virtual void OnEndPlay()	  {}
	virtual void OnStartDestroy() {}

public:

	virtual void Tick(float DeltaTime, ETickType type);

protected:
	/// >>
	UNIQUE(World)     world;
	SimulationMode*   simulationMode;
	PlayerController* playerController;
	/// << 

public: //~~~~~~~~~~~~~~| Creation functions

	template<class _T>
	_T* CreateObject(std::string name)
	{
		return ObjectCreator::CreateObject<_T>(name, world.get());
	}

	template<class _T>
	_T* CreateActor(std::string name,  bool AttachToController = false)
	{
		return ObjectCreator::CreateActor<_T>(name, world.get(), AttachToController ? playerController : nullptr);
	}
};



#endif //!GAME_MODE_HPP