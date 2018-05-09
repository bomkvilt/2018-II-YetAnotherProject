#pragma once
#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <vector>
#include <memory>

#include "ActorComponent.hpp"
#include "ActorModule.hpp"
#include "Object.hpp"


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

	template<class _T, typename... Args>
	_T* CreateSubcomponent(std::string name, Args&... args)
	{
		if (auto* point = ObjectCreator::CreateSubcomponent<_T>(name, world, this, args...))
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

	template<class _T, typename... Args>
	_T* CreateSubModule(std::string name, Args&... args)
	{
		if (auto* point = ObjectCreator::CreateSubmodule<_T>(name, world, this, args...))
		{
			modules.emplace_back(point);
			return point;
		}
		return nullptr;
	}
};


#endif // !ACTOR_HPP