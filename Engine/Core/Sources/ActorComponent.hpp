#ifndef ACTOR_COMPONENT_HPP
#define ACTOR_COMPONENT_HPP

#include <string>
#include <memory>
#include <vector>

#include "Object.hpp"
#include "Interfaces/IFacade.hpp"
#include "Interfaces/IRigidBody.hpp"

class Actor;
class World;
class PlayerController;


/** All object components in the world makes a backet tree (NODE: Actor is a kind of wrapper on components)
 *	Each Component have:
 *	. realtive transform 
 *	. absolute transform
 *	
 *	. subcomponent	- child nodes probably dependesce from the one
 *	. parent		- a parent node
 *	. owner			- an objects associated with the component
 *	. world			- a world where the component is placed
 */
class ActorComponent : public Object
{
	GENERATED_BODY(ActorComponent, Object)
public:

	ActorComponent();

public: //~~~~~~~~~~~~~~| Physics

	void AddForce (const FVector& force, ESpaceType space);
	void AddTorque(const FVector& torue, ESpaceType space);

public: //~~~~~~~~~~~~~~| Position

	/// transform

	void SetComponentTransform(FTransform newTransform);
	void SetRelativeTransform (FTransform newTransform);
	FTransform GetComponentTransform() const { return worldTransform;    }
	FTransform GetRelativeTransform () const { return relativeTarnsform; }

	/// location

	void SetComponentLocation(FVector newLocation);
	void SetRelativeLocation (FVector newLocation);
	FVector  GetComponentLocation() const { return worldTransform   .Location; }
	FVector  GetRelativeLocation () const { return relativeTarnsform.Location; }

	/// rotation

	void SetComponentRotation(FQuat newRotation);
	void SetRelativeRotation (FQuat newRotation);
	FQuat GetComponentRotation() const { return worldTransform   .Rotation; }
	FQuat GetRelativeRotation () const { return relativeTarnsform.Rotation; }

	/// add

	void AddTransform		 (FTransform delta, ESpaceType space = ESpaceType::eWorld);
	void AddComponentLocation(FVector	 delta, ESpaceType space = ESpaceType::eWorld);
	void AddComponentRotation(FQuat      delta, ESpaceType space = ESpaceType::eWorld);

public: //~~~~~~~~~~~~~~| Chain

	/// owner

	const Actor* GetOwner() const	{ return owner; }
		  Actor* GetOwner()			{ return owner; }
	void SetOwner(Actor* newOwner)	{ owner = newOwner; }

	/// if owner is Avatar

	const PlayerController* GetPlayerController() const;
		  PlayerController* GetPlayerController();
	
	void AttachTo(ActorComponent* newParent);
	void Detach();

	/// facade

	const IFacade* GetFacade() const { return facade.get(); }
          IFacade* GetFacade()		 { return facade.get(); }

	/// rigid body

	const IRigidBody* GetRigidBody() const { return rigidBody.get(); }
		  IRigidBody* GetRigidBody()       { return rigidBody.get(); }
		  
	/// parent

	const ActorComponent* GetParent() const		{ return parent; }
	      ActorComponent* GetParent()			{ return parent; }

	/// components

	      std::vector<ActorComponent*>& GetSubcomponents();
	const std::vector<ActorComponent*>& GetSubcomponents() const;
	
protected:

	void Internal_GetSubcomponents(std::vector<		 ActorComponent*>& components);
	void Internal_GetSubcomponents(std::vector<const ActorComponent*>& components) const;

	void UpdateFacade();

	void UpdateWoldTransform();
	void UpdateRelativeTransform();
	void UpdateChainTransforms();

	FTransform GetParentTransform() const;

protected:

	/// >> common 
	Actor*	owner;
	/// << 

	/// << modules
	UNIQUE(IFacade)		facade;
	UNIQUE(IRigidBody)	rigidBody;
	///

	/// >> position
	FTransform relativeTarnsform;
	FTransform worldTransform;
	/// <<

	/// >> tree
	ActorComponent* parent;
	std::vector<ActorComponent*> subcomponents;
	/// <<
};

#endif // !ACTOR_COMPONENT_HPP