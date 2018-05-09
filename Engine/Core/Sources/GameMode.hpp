#ifndef GAME_MODE_HPP
#define GAME_MODE_HPP
#pragma once

#include "ObjectCreator.hpp"
#include "Common.hpp"
#include "World.hpp"

class SimulationMode;


class GameMode
{
public:

	GameMode();
	
	virtual void OnConstructed()  {}
	virtual void OnBeginPlay();
	virtual void OnEndPlay();
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

	template<class _T, typename... Args>
	_T* CreateObject(std::string name, Args&... args)
	{
		return ObjectCreator::CreateObject<_T>(name, world.get(), args...);
	}

	template<class _T, typename... Args>
	_T* CreateActor(std::string name, Args&... args)
	{
		return ObjectCreator::CreateActor<_T>(name, world.get(), args...);
	}

	template<class _T, typename... Args>
	_T* CreateAvatar(std::string name,  bool AttachToController = false, Args&... args)
	{
		return ObjectCreator::CreateAvatar<_T>(name, world.get(), AttachToController ? playerController : nullptr, args...);
	}
};



#endif //!GAME_MODE_HPP