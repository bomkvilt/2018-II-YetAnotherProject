#ifndef B2RIGID_BODY_HPP
#define B2RIGID_BODY_HPP

#include "Box2D/Box2D.h"

#include "Interfaces/Default/RigidBody.hpp"
#include "B2Convertion.hpp"
#include "Common.hpp"

struct RigidBody : public FRigidBody
{
public:
	RigidBody(FShape shape, BaseActorComponent* owner, float mass, FVector inertia);
	~RigidBody();

	virtual void Update() override;
	virtual void Sync()   override;

public:
	virtual void AddForce          (const FVector& force  ) override;
	virtual void AddTorque         (const FVector& torque ) override;
	virtual void AddImpulce        (const FVector& impulce) override;
	virtual void AddKineticMomement(const FVector& moment ) override;

public:
	virtual void    SetMass    (float newMass     )       override;
	virtual void    SetInertia (FVector newInertia)       override;
	virtual float   GetMass    ()                   const override;
	virtual FVector GetInertia ()                   const override;
	
	virtual void SetExtents(FVector newExtents) override;

	virtual FVector GetVelocity()                     const override;
	virtual void    SetVelocity(const FVector& newVelocity) override;
	
	virtual FVector GetOmega()                  const override;
	virtual void    SetOmega(const FVector& newOmega) override;

public:
	b2Transform GetTransform() const;
	void UpdateState();

public:
	float          density;   // helper << 
	float          square;    // helper << square of a collision
	EShapeType     shapeType; // helper << collision shape type
	b2MassData     massData;  // helper << 
	b2PolygonShape collision; // helper << TODO:: make b2Shape
	b2Body*        rigidBody; //        << NOTE:: the object creates in a scene
	b2Fixture*     fixture;   //        << 
};



#endif // !B2RIGID_BODY_HPP
