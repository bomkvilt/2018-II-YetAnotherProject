#ifndef PHYSICS_FABRIC_HPP
#define PHYSICS_FABRIC_HPP
#pragma once

#include "Interfaces/IPhysicsScene.hpp"
#include "Interfaces/IConstraint.hpp"
#include "Interfaces/IRigidBody.hpp"


struct IPhysicsFabric
{
	virtual UNIQUE(IPhysicsScene) MakePhysicsScene() = 0;
	virtual UNIQUE(IConstraint  ) MakeConstraint(BaseActorComponent* owner) = 0;
	virtual UNIQUE(IRigidBody   ) MakeRigidBody(FShape shape, BaseActorComponent* owner, float  mass = 0, FVector inertia = FVector::ZeroVector) = 0;
};



#endif // !PHYSICS_FABRIC_HPP
