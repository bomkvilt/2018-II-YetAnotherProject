#pragma once


#include "Actor.hpp"
#include "World.hpp"
#include "PlayerController.hpp"


/** A class handling a symualtion
 *	Tasks:
 *	. world initialisation
 *	. 
 */
class SymulationMode
{
public:

	SymulationMode();
	virtual ~SymulationMode();

	static UNIQUE(SymulationMode) Get();

public:

	virtual void OnSymulationBegin();
	virtual void OnSymulationEnd();

	virtual void DoTick(float deltaTime, ETickType type);
	
	virtual void StopSymulation();

	virtual bool TickRequired() const;

protected: //~~~~~~~~~~~~~~| component setup
	
	static World*			 MakeWorld();
	
private:

	/** The functions are private couse:
	 * . we need to 
	 */

	static PlayerController* MakeController();

public: //~~~~~~~~~~~~~~| Time

	float GetSymulationStep();

	void SetDeltaTime(float delta);

protected:

	/// >> tick
	bool bStop;
	/// << 

	/// >> time (seconds)
	float symlationStep;
	float currentTime;
	float lastTime;
	/// <<

	/// >>
	UNIQUE(World)			 world;
	UNIQUE(PlayerController) controller;
	/// <<

public: //~~~~~~~~~~~~~~| Templates

	template<class _T>
	_T* CreateActor(std::string name, bool AttachToController = false)
	{
		assert(world);

		auto* init = new Initialiser;
		init->name = &name;
		init->world	= world.get();

		if (AttachToController)
		{
			init->controller = controller.get();
		}

		ThreadContextSingleton::Get().PushInitialiser(init);
		auto* point = world->CreateObject<_T>();
		ThreadContextSingleton::Get().PopInitialiser();

		if (AttachToController && controller)
		{
			controller->SetTarget(point);
		}


		return point;
	}
};