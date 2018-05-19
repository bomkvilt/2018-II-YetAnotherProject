#pragma once
#ifndef IRIGID_BODY_HPP
#define IRIGID_BODY_HPP

#include "Common.hpp"

class BaseActorComponent;
class World;


enum class ERigidBodyType
{
	  eStatic    // translation
	, eKinematic // translation + velocity
	, eDynamic   // translation + velocity + forces
};


/** 
 * collisionType(BodyA, BodyB) = max(type_A, type_B);
 */
enum ECollisiontype
{
	  eCollide = 0 // 
	, eOverlap = 1 // 
	, eIgnore  = 2 // 
};


struct FCollisionRules 
{
	using Rules = std::unordered_map<int, ECollisiontype>;

	static const int maxCategoryCount = 16;

public:
	int category;
	int rules[maxCategoryCount];

public:
	FCollisionRules()
		: category(0)
	{
		memset(rules, 0, maxCategoryCount * sizeof(int));
	}
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

public: //~~~~~~~~~~~~~~| owner
	/// owner
	virtual const BaseActorComponent* GetOwner() const   = 0;
	virtual       BaseActorComponent* GetOwner()         = 0;
	virtual void  SetOwner(BaseActorComponent* newOwner) = 0;

public: //~~~~~~~~~~~~~~| shape
	/// mass
	virtual float   GetMass    ()             const = 0;
	virtual FVector GetInertia ()             const = 0;
	virtual void    SetMass    (float   newMass   ) = 0;
	virtual void    SetInertia (FVector newInertia) = 0;
	/// extents
	virtual void    SetExtents (FVector newExtents) = 0;

public: //~~~~~~~~~~~~~~| velocity
	/// velocity
	virtual FVector GetVelocity()                     const = 0;
	virtual void    SetVelocity(const FVector& newVelocity) = 0;
	/// omega
	virtual FVector GetOmega()                  const = 0;
	virtual void    SetOmega(const FVector& newOmega) = 0;

public: //~~~~~~~~~~~~~~| body
	///body type
	virtual ERigidBodyType GetBodyType()       const = 0;
	virtual void SetBodyType(ERigidBodyType newType) = 0;
	///collision
	virtual FCollisionRules GetRules()                        const = 0;
	virtual void SetCollisionRules(const FCollisionRules& newRules) = 0;
};




#endif // !IRIGID_BODY_HPP
