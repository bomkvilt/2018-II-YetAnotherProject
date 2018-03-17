#pragma once

#include <vector>
#include <memory>

#include "btBulletDynamicsCommon.h"

#include "ActorComponent.hpp"
#include "Object.hpp"


/** Base class for all Actors on scene
 *	Privides:
 *	. relative location	(root component driven)
 *	. obsolute location	(root component driven)
 */
class Actor : public Object
{
	using Super = Object;

public:

	Actor();
	~Actor();

public: //~~~~~~~~~~~~~~| Physics -> to root component

	void AddForce (const btVector3& force, ESpaceType space);
	void AddTorque(const btVector3& torue, ESpaceType space);

protected:

	/// >> common
	ActorComponent* rootComponent;
	/// <<

public: //~~~~~~~~~~~~~~| Templates

	template<class _T>
	_T* CreateSubComponent(std::string name)
	{
		assert(world);

		auto* init = new Initialiser;
		init->name = &name;
		init->world = world;
		init->owner = this;

		ThreadContextSingleton::Get().PushInitialiser(init);

		auto* point = world->CreateObject<_T>();
		
		ThreadContextSingleton::Get().PopInitialiser();


		if (this->rootComponent)
		{
			point->AttachTo(rootComponent);
		}
		else
		{
			point->AttachTo(world->GetSceneRoot());
			rootComponent = point;
		}

		return point;
	}
};
