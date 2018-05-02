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

	void AddForce (const FVector& force, ESpaceType space);
	void AddTorque(const FVector& torue, ESpaceType space);

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
	_T* CreateSubComponent(std::string name)
	{
		return ObjectCreator::CreateSubComponent<_T>(name, world, this);
	}
};
