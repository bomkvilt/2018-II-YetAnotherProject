#pragma once

#include <string>
#include <memory>
#include <vector>


#include "Object.hpp"
#include "Facade.hpp"


class Actor;
class World;
class PlayerController;


/** All object components in the world makes a backet tree (NODE: Actor is a kind of wrapper on components)
 *	Each Component have:
 *	. realtive transform (uses to set world position)
 *	. absolute transform (updates with world)
 *	
 *	. subcomponent	- child nodes probably dependesce from the one
 *	. parent		- a parent node
 *	. owner			- an objects associated with the component
 *	. world			- a world where the component is placed
 */
class ActorComponent : public Object
{
	using Super = Object;

public:

	ActorComponent();

public: //~~~~~~~~~~~~~~| Physics

	void AddForce (const Vector3f& force, ESpaceType space);
	void AddTorque(const Vector3f& torue, ESpaceType space);

public: //~~~~~~~~~~~~~~| Position

	/// transform

	void SetComponentTransform(Transform newTransform);
	void SetRelativeTransform (Transform newTransform);

	Transform GetComponentTransform() const { return worldTransform;	  }
	Transform GetRelativeTransform () const { return relativeTarnsform; }

	/// location

	Vector3f  GetComponentLocation() const { return worldTransform   .getOrigin(); }
	Vector3f  GetRelativeLocation () const { return relativeTarnsform.getOrigin(); }

	/// rotation

	Quatf GetComponentRotation() const { return worldTransform   .getRotation(); }
	Quatf GetRelativeRotation () const { return relativeTarnsform.getRotation(); }

	/// add

	void AddTransform		 (Transform delta, ESpaceType space = ESpaceType::eWorld);
	void AddComponentLocation(Vector3f	delta, ESpaceType space = ESpaceType::eWorld);
	void AddComponentRotation(Quatf     delta, ESpaceType space = ESpaceType::eWorld);

public: //~~~~~~~~~~~~~~| Chain

	/// owner

	const Actor* GetOwner() const	{ return owner; }
		  Actor* GetOwner()			{ return owner; }
	void SetOwner(Actor* newOwner)	{ owner = newOwner; }

	/// if owner is Avatar

	const PlayerController* GetPlayerController() const;
		  PlayerController* GetPlayerController();

	/// parent

	const ActorComponent* GetParent() const		{ return parent; }
		  ActorComponent* GetParent()			{ return parent; }
	
	void AttachTo(ActorComponent* newParent);
	void Detach();

	/// facade

	const Facade& GetFacade() const { return facade; }
		  Facade& GetFacade()		{ return facade; }

	/// components

	std::vector<	  ActorComponent*> GetSubcomponents();
	std::vector<const ActorComponent*> GetSubcomponents() const;
	
protected:

	void Internal_GetSubcomponents(std::vector<		 ActorComponent*>& components);
	void Internal_GetSubcomponents(std::vector<const ActorComponent*>& components) const;

	void UpdateFacade();

	void UpdateWoldTransform();
	void UpdateRelativeTransform();
	void UpdateChainTransforms();

	Transform GetParentTransform() const;

protected:

	/// >> common 
	Actor*	owner;
	/// << 

	/// << grafics
	Facade facade;
	///

	/// >> position
	Transform relativeTarnsform;
	Transform worldTransform;
	/// <<

	/// >> tree elements
	ActorComponent* parent;
	std::vector<ActorComponent*> subcomponents;
	/// <<
};