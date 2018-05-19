#pragma once
#ifndef IRIGID_BODY_HPP
#define IRIGID_BODY_HPP

#include "Common.hpp"

class BaseActorComponent;
class World;


enum class ERigidBodyType
{
	  eStatic    // Satic object
	, eKinematic // Ignores forces
	, eDynamic	 // Dynamic objetc
	, eIgnore	 // Slip physics
};



struct IRigidBody
{
public:
	virtual ~IRigidBody() {}

	virtual void Update() = 0; // sync the body  (owner's transform -> body)
	virtual void Sync()   = 0; // sync the owner (body's transform -> owner)

public: //~~~~~~~~~~~~~~| force / impulce
	virtual void AddForce          (const FVector& force  ) = 0;
	virtual void AddTorque         (const FVector& torque ) = 0;
	virtual void AddImpulce        (const FVector& impulce) = 0;
	virtual void AddKineticMomement(const FVector& moment ) = 0;

public: //~~~~~~~~~~~~~~| 
	/// owner
	virtual const BaseActorComponent* GetOwner() const   = 0;
	virtual       BaseActorComponent* GetOwner()         = 0;
	virtual void  SetOwner(BaseActorComponent* newOwner) = 0;
	/// mass
	virtual float   GetMass    ()             const = 0;
	virtual FVector GetInertia ()             const = 0;
	virtual void    SetMass    (float   newMass   ) = 0;
	virtual void    SetInertia (FVector newInertia) = 0;
	/// extents
	virtual void    SetExtents(FVector newExtents) = 0;
	
	//virtual bool    GetProcessed
	//virtual void    SetProcessed() const = 0;
	
	/// velocity
	virtual FVector GetVelocity()                     const = 0;
	virtual void    SetVelocity(const FVector& newVelocity) = 0;
	/// omega
	virtual FVector GetOmega()                  const = 0;
	virtual void    SetOmega(const FVector& newOmega) = 0;
	///body type
	virtual ERigidBodyType GetBodyType()       const = 0;
	virtual void SetBodyType(ERigidBodyType newType) = 0;
};




#endif // !IRIGID_BODY_HPP
