#pragma once
#ifndef B2FABRIC_HPP
#define B2FABRIC_HPP

#include "Injection/IPhysicsFabric.hpp"
#include "B2PhysicsScene.hpp"
#include "B2Constraint.hpp"
#include "B2RigidBody.hpp"


class B2Fabric : public IPhysicsFabric
{
public:
	static UNIQUE(IPhysicsFabric) Get()
	{
		return std::make_unique<B2Fabric>();
	}

	virtual UNIQUE(IPhysicsScene) MakePhysicsScene() override
	{
		return std::make_unique<PhysicsScene>();
	}

	virtual UNIQUE(IConstraint) MakeConstraint(ActorComponent* owner) override
	{
		return std::make_unique<Constraint>(owner);
	}

	virtual UNIQUE(IRigidBody) MakeRigidBody(FShape shape, ActorComponent* owner, float mass, FVector inertia) override
	{
		return std::make_unique<RigidBody>(shape, owner, mass, inertia);
	}
};

#endif // !B2FABRIC_HPP