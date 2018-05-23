#ifndef BASE_ACTOR_COMPONENT_HPP
#define BASE_ACTOR_COMPONENT_HPP
#pragma once

#include <string>
#include <vector>

#include "Object.hpp"
#include "Interfaces/IFacade.hpp"
#include "Interfaces/IRigidBody.hpp"
#include "Interfaces/IWorlObject.hpp"
#include "Delegation/Delegate.hpp"

class Actor;
class PlayerController;
struct IRigidBody;



class BaseActorComponent : public Object
	, public IWorldObject
{
	GENERATED_BODY(BaseActorComponent, Object)
public:

	BaseActorComponent();

public: //~~~~~~~~~~~~~~| events

	using DL_OnCollisionEnter = Delegate<Actor* /*Other actor*/, BaseActorComponent* /*other comp*/, FHit>;
	using DL_OnCollisionExit  = Delegate<Actor* /*Other actor*/, BaseActorComponent* /*other comp*/>;

	DL_OnCollisionEnter OnCollisionEnter;
	DL_OnCollisionExit  OnCollisionExit;

public: //~~~~~~~~~~~~~~| Physics

	void AddForce          (const FVector& force,   ESpaceType space);
	void AddTorque         (const FVector& torque,  ESpaceType space);
	void AddImpulce        (const FVector& impulce, ESpaceType space);
	void AddKineticMomement(const FVector& moment,  ESpaceType space);

	FVector GetVelocity() const;
	FVector GetOmega()    const;

	float GetMass() const;
	void SetMass(float newMass);

	FVector GetInertia() const;
	void SetInertia(FVector newInertia);

public: //~~~~~~~~~~~~~~| Position
	/** NOTE:
	 * @param bExcludePhysics - if true child dynamic object won't be updated
	 * @param bUpdateBody     - if true - update a rigid body
	 */

	/// transform

	void SetComponentTransform(FTransform newTransform, bool bExcludePhysics, bool bUpdateBody);
	void SetRelativeTransform (FTransform newTransform, bool bExcludePhysics, bool bUpdateBody);

	virtual FTransform GetComponentTransform() const override { return worldTransform;    }
	virtual FTransform GetRelativeTransform () const override { return relativeTarnsform; }

	virtual void SetComponentTransform(FTransform newTransform) override { SetComponentTransform(newTransform, false, true); }
	virtual void SetRelativeTransform (FTransform newTransform) override { SetRelativeTransform (newTransform, false, true); }

	/// location

	void SetComponentLocation(FVector newLocation, bool bExcludePhysics, bool bUpdateBody);
	void SetRelativeLocation (FVector newLocation, bool bExcludePhysics, bool bUpdateBody);

	virtual FVector GetComponentLocation() const override { return worldTransform   .Location; }
	virtual FVector GetRelativeLocation () const override { return relativeTarnsform.Location; }

	virtual void SetComponentLocation(FVector newLocation) override { SetComponentLocation(newLocation, false, true); }
	virtual void SetRelativeLocation (FVector newLocation) override { SetRelativeLocation (newLocation, false, true); }

	/// rotation

	void SetComponentRotation(FQuat newRotation, bool bExcludePhysics, bool bUpdateBody);
	void SetRelativeRotation (FQuat newRotation, bool bExcludePhysics, bool bUpdateBody);

	virtual FQuat GetComponentRotation() const override { return worldTransform   .Rotation; }
	virtual FQuat GetRelativeRotation () const override { return relativeTarnsform.Rotation; }

	virtual void SetComponentRotation(FQuat newRotation) override { SetComponentRotation(newRotation, false, true); }
	virtual void SetRelativeRotation (FQuat newRotation) override { SetRelativeRotation (newRotation, false, true); }

	/// add

	void AddTransform        (FTransform delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody);
	void AddComponentLocation(FVector    delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody);
	void AddComponentRotation(FQuat      delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody);

	virtual void AddTransform        (FTransform delta, ESpaceType space = ESpaceType::eWorld) override { AddTransform        (delta, space, false, true); };
	virtual void AddComponentLocation(FVector    delta, ESpaceType space = ESpaceType::eWorld) override { AddComponentLocation(delta, space, false, true); };
	virtual void AddComponentRotation(FQuat      delta, ESpaceType space = ESpaceType::eWorld) override { AddComponentRotation(delta, space, false, true); };

public: //~~~~~~~~~~~~~~| Chain

	/// owner

	const Actor* GetOwner() const	{ return owner; }
		  Actor* GetOwner()			{ return owner; }
	void SetOwner(Actor* newOwner)	{ owner = newOwner; }
	
	void AttachTo(BaseActorComponent* newParent);
	void Detach();

	/// facade

          IFacade* GetFacade()		 { return facade.get(); }
	const IFacade* GetFacade() const { return facade.get(); }

	/// rigid body

		  IRigidBody* GetRigidBody()       { return rigidBody.get(); }
	const IRigidBody* GetRigidBody() const { return rigidBody.get(); }
	
	bool IsDynamic() const;
		  
	/// parent

	      BaseActorComponent* GetParent()			{ return parent; }
	const BaseActorComponent* GetParent() const		{ return parent; }

	/// components

	      std::vector<BaseActorComponent*>& GetSubcomponents();
	const std::vector<BaseActorComponent*>& GetSubcomponents() const;

public: //~~~~~~~~~~~~~~| Misc



		virtual FVector GetExtents() const { return extents; }
		virtual void SetExtents(const FVector& newExtents) { extents = newExtents; }

	FVector SpaceToWorld(const FVector& v, ESpaceType space) const;
	FQuat   SpaceToWorld(const FQuat&   v, ESpaceType space) const;

	FVector SpaceToLocal(const FVector& v, ESpaceType space) const;
	FQuat   SpaceToLocal(const FQuat&   v, ESpaceType space) const;
	
protected:
	/** NOTE:
	* @param bExcludePhysics - if true child dynamic object won't be updated
	* @param bUpdateBody     - if true - update a rigid body
	*/

	void AddSubcomponent   (BaseActorComponent* child);
	void RemoveSubcomponent(BaseActorComponent* child);

	void Internal_GetSubcomponents(std::vector<		 BaseActorComponent*>& components);
	void Internal_GetSubcomponents(std::vector<const BaseActorComponent*>& components) const;

	void UpdateFacade();
	void UpdateBody();

	void UpdateWoldTransform    (bool bExcludePhysics, bool bUpdateBody);
	void UpdateRelativeTransform(bool bExcludePhysics, bool bUpdateBody);
	void UpdateChainTransforms  (bool bExcludePhysics, bool bUpdateBody);

	FTransform GetParentTransform() const;

protected:

	virtual void OnSubcomponentAttached(BaseActorComponent* child ) {}
	virtual void OnSubcomponentDetached(BaseActorComponent* child ) {}
	virtual void OnComponentAttached   (BaseActorComponent* parent) {}
	virtual void OnComponentDetached   (BaseActorComponent* parent) {}
	virtual void onSubcomponentMoved   (BaseActorComponent* child, bool bUpdateBody) {}
	virtual void onComponentMoved      (bool bUpdateBody);   

protected:

	/// >> common 
	Actor*	owner;
	/// << 
	FVector extents;
	/// << modules
	UNIQUE(IFacade)		facade;
	UNIQUE(IRigidBody)	rigidBody;
	///

	/// >> position
	FTransform relativeTarnsform;
	FTransform worldTransform;
	/// <<

	/// >> tree
	BaseActorComponent* parent;
	std::vector<BaseActorComponent*> subcomponents;
	/// <<
};

#endif // !BASE_ACTOR_COMPONENT_HPP