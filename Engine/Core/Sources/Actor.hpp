#pragma once

#include <vector>
#include <memory>

#include "ActorComponent.hpp"
#include "ActorModule.hpp"
#include "Object.hpp"


/** Base class for all Actors on scene
 *	Privides:
 *	. relative location	(root component driven)
 *	. obsolute location	(root component driven)
 */
class Actor : public Object
{
	GENERATED_BODY(Actor, Object)
public:

	Actor();

public: //~~~~~~~~~~~~~~| Physics -> to root component

	void AddForce          (const FVector& force,   ESpaceType space);
	void AddTorque         (const FVector& torque,  ESpaceType space);
	void AddImpulce        (const FVector& impulce, ESpaceType space);
	void AddKineticMomement(const FVector& moment,  ESpaceType space);

public: //~~~~~~~~~~~~~~| Kinematic -> to root component

		/// transform

	void SetComponentTransform(FTransform newTransform);
	void SetRelativeTransform (FTransform newTransform);
	FTransform GetComponentTransform() const;
	FTransform GetRelativeTransform()  const;

	/// location

	void SetComponentLocation(FVector newLocation);
	void SetRelativeLocation (FVector newLocation);
	FVector GetComponentLocation() const;
	FVector GetRelativeLocation()  const;

	/// rotation

	void SetComponentRotation(FQuat newRotation);
	void SetRelativeRotation (FQuat newRotation);
	FQuat GetComponentRotation() const;
	FQuat GetRelativeRotation()  const;

	/// add

	void AddTransform        (FTransform delta, ESpaceType space = ESpaceType::eWorld);
	void AddComponentLocation(FVector    delta, ESpaceType space = ESpaceType::eWorld);
	void AddComponentRotation(FQuat      delta, ESpaceType space = ESpaceType::eWorld);

public: //~~~~~~~~~~~~~~| chain and modules

	void AttachTo(Actor* newParent);
	void Detach();

	const ActorComponent* GetRootComponent() const { return rootComponent; }
	      ActorComponent* GetRootComponent()       { return rootComponent; }
	void SetRootComponent(ActorComponent* newRoot);

	      std::vector<ActorModule*>& GetModules()       { return modules; }
	const std::vector<ActorModule*>& GetModules() const { return modules; }

protected:

	/// >> common
	ActorComponent* rootComponent;
	std::vector<ActorModule*> modules;
	/// <<	

public: //~~~~~~~~~~~~~~| Creation functions

	template<class _T>
	_T* CreateSubcomponent(std::string name)
	{
		if (auto* point = ObjectCreator::CreateSubcomponent<_T>(name, world, this))
		{
			if (rootComponent)
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
		return nullptr;
	}

	template<class _T>
	_T* CreateSubModule(std::string name)
	{
		if (auto* point = ObjectCreator::CreateSubmodule<_T>(name, world, this))
		{
			modules.emplace_back(point);
			return point;
		}
		return nullptr;
	}
};
