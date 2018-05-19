#pragma once

#include <vector>
#include <memory>

#include "BaseActorComponent.hpp"
#include "ActorModule.hpp"
#include "Object.hpp"


/** Base class for all Actors on scene
*	Privides:
*	. relative location	(root component driven)
*	. obsolute location	(root component driven)
*/
class Actor : public Object
	, public IWorldObject
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

	virtual void SetComponentTransform(FTransform newTransform) override;
	virtual void SetRelativeTransform (FTransform newTransform) override;
	virtual FTransform GetComponentTransform() const override;
	virtual FTransform GetRelativeTransform()  const override;

	/// location

	virtual void SetComponentLocation(FVector newLocation) override;
	virtual void SetRelativeLocation (FVector newLocation) override;
	virtual FVector GetComponentLocation() const override;
	virtual FVector GetRelativeLocation()  const override;

	/// rotation

	virtual void SetComponentRotation(FQuat newRotation);
	virtual void SetRelativeRotation (FQuat newRotation);
	virtual FQuat GetComponentRotation() const override;
	virtual FQuat GetRelativeRotation()  const override;

	/// add

	virtual void AddTransform        (FTransform delta, ESpaceType space = ESpaceType::eWorld) override;
	virtual void AddComponentLocation(FVector    delta, ESpaceType space = ESpaceType::eWorld) override;
	virtual void AddComponentRotation(FQuat      delta, ESpaceType space = ESpaceType::eWorld) override;

public: //~~~~~~~~~~~~~~| chain and modules

	void AttachTo(Actor             * newParent);
	void AttachTo(BaseActorComponent* newParent);
	void Detach();

	const BaseActorComponent* GetRootComponent() const { return rootComponent; }
	      BaseActorComponent* GetRootComponent()       { return rootComponent; }
	void SetRootComponent(BaseActorComponent* newRoot);


	std::vector<ActorModule*>& GetModules()       { return modules; }
	const std::vector<ActorModule*>& GetModules() const { return modules; }

protected:

	/// >> common
	BaseActorComponent* rootComponent;
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

private:
	friend struct ObjectLocker;

	std::mutex _lock;
	
	std::unique_lock<std::mutex> lock;
};
