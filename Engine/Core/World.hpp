#pragma once

#include <assert.h>

#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "btBulletDynamicsCommon.h"

#include "../Types.hpp"
#include "../Misc.hpp"
#include "ObjectBase.hpp"

#include "Threading/Initialiser.hpp"


class Actor;
class Object;
class ActorComponent;


/** The world is a wrapper of a phisics scene
 *	
 ** Object manageing:
 *	. creates and own all objects
 *	. creates and own all objectsComponents
 */
class World
{
public:
			 World();
	virtual ~World();

public:

	void OnSymulationStart();
	void OnSymulationStop();

public: //~~~~~~~~~~~~~~| Tick

	void DoTick(float DeltaTime, ETickType type);

	void RegisterTickFunction  (ITickFunction& Tick);
	void UnregisterTickFunction(ITickFunction& Tick);

public: //~~~~~~~~~~~~~~| Construction

	template<class _T>
	_T* CreateObject()
	{
		std::string& name = *ThreadContextSingleton::Get().TopInitialiser()->name;
		UpdateNameToUnique(name);

		OUID newOUID = lastOUID++;

		ThreadContextSingleton::Get().TopInitialiser()->ouid = newOUID;

		auto base = UNIQUE(ObjectBase)(new _T);

		auto* pointer_base = base.get();
		auto* pointer = static_cast<_T*>(pointer_base);

		objects.emplace(newOUID, std::move(base));
		objects_set.emplace(pointer);

		pointer->OnConstructed();

		if (symulationState != eUnstarted)
		{
			pointer->OnBeginPlay();
		}

		return pointer;
	}

	void DestroyObject(Object* object);

public:

	bool IsValid(Object* base) const;

public: //~~~~~~~~~~~~~~| 

	const ActorComponent* GetSceneRoot() const	{ return sceneRoot; }
		  ActorComponent* GetSceneRoot()		{ return sceneRoot; }

protected:

	void UpdateNameToUnique(std::string& name);

protected:

	/// >>
	ESymulationState symulationState;
	/// << 

	/// >> physics
	UNIQUE(btDiscreteDynamicsWorld) PxWorld;
	/// << 

	/// >> graphic
	ActorComponent* sceneRoot;
	/// <<

	/// >> indices
	std::unordered_set<Object*> objects_set;
	std::unordered_map<std::string, Index> names;

	OUID lastOUID;

	std::unordered_map<OUID, UNIQUE(ObjectBase), std::hash<size_t>> objects;
	/// <<

	/// >> tick functions
	std::array< 
		std::unordered_set< ITickFunction* >, 
		ETickType::eMAX
	> tickFunctions;
	/// <<
};
