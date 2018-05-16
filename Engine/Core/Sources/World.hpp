#ifndef WORLD_HPP
#define WORLD_HPP
#pragma once

#include <array>
#include <unordered_map>
#include <unordered_set>

#include "ObjectBase.hpp"

#include "Interfaces/IPhysicsScene.hpp"

#include "Threading/Initialiser.hpp"


class Actor;
class Object;
class BaseActorComponent;


/** 
 */
class World
{
public:
			 World();
	virtual ~World();

public:

	void OnSimulationStart();
	void OnSimulationStop();

public: //~~~~~~~~~~~~~~| Tick

	void DoTick(float DeltaTime, ETickType type);

	void RegisterTickFunction  (ITickFunction& Tick);
	void UnregisterTickFunction(ITickFunction& Tick);

public: //~~~~~~~~~~~~~~| Construction

	template<class _T>
	_T* CreateObject() //TODO:: make a review
	{
		// make the name unique
		auto* initialiser = ThreadContext::TopInitialiser();
		assert(initialiser);  auto* name = initialiser->name;
		assert(name       );  UpdateNameToUnique(*name);

		// create a new uid
		OUID newOUID = lastOUID++;
		initialiser->ouid = newOUID;

		// create a new object
		auto base = UNIQUE(ObjectBase)(new _T);

		auto* pointer_base = base.get();
		auto* pointer = static_cast<_T*>(pointer_base);
		
		objects.emplace(newOUID, std::move(base));
		objects_set.emplace(pointer);

		pointer->OnConstructed();

		if (SimulationState != eUnstarted)
		{
			pointer->OnBeginPlay();
		}

		return pointer;
	}

	void DestroyObject(Object* object);

public:

	bool IsValid(Object* base) const;

public: //~~~~~~~~~~~~~~| 

<<<<<<< HEAD
		  BaseActorComponent* GetSceneRoot()		{ return sceneRoot; }
	const BaseActorComponent* GetSceneRoot() const	{ return sceneRoot; }

	      IPhysicsScene* GetPhysicsScene()       { return scene.get(); }
	const IPhysicsScene* GetPhysicsScene() const { return scene.get(); }
=======
		  ActorComponent* GetSceneRoot()		{ return sceneRoot; }
	const ActorComponent* GetSceneRoot() const	{ return sceneRoot; }
>>>>>>> origin/sfml

protected:

	void UpdateNameToUnique(std::string& name);

protected:

	/// >>
	ESimulationState SimulationState;
	/// << 

	/// >> scene
<<<<<<< HEAD
	BaseActorComponent*   sceneRoot;
	UNIQUE(IPhysicsScene) scene;
=======
	ActorComponent* sceneRoot;
>>>>>>> origin/sfml
	/// <<

	/// >> indices
	OUID lastOUID;
	
	std::unordered_set<Object*>      objects_set;
	std::unordered_map<std::string, Index> names;

	std::unordered_map< OUID
		, UNIQUE(ObjectBase)
		, std::hash<size_t>
	> objects;
	/// <<

	/// >> tick functions
	std::array< 
		std::unordered_set< ITickFunction* >, 
		ETickType::eMAX
	> tickFunctions;
	/// <<

public: //~~~~~~~~~~~~~~| Iteration

	class SceneIterator
	{
	public:
		SceneIterator(BaseActorComponent* root);

		BaseActorComponent* operator->();
		BaseActorComponent& operator* ();
		SceneIterator*  operator++();
		bool operator==(const SceneIterator& r) const;
		bool operator!=(const SceneIterator& r) const;

	private:

		BaseActorComponent* PrevNode();
		BaseActorComponent* CurrNode();
		BaseActorComponent* Brunch(size_t node);
		std::vector<BaseActorComponent*>& CurrLine();

		size_t CurrIndex();
		size_t MaxIndex();
		void Move();
		
		std::stack<size_t> indices;
		std::stack<BaseActorComponent*> path;
	};

	SceneIterator begin() { return SceneIterator(sceneRoot); }
	SceneIterator end()   { return SceneIterator(nullptr);   }
};

#endif //!WORLD_HPP