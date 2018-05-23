#pragma once
#ifndef PHYSICS_SCENE_HPP
#define PHYSICS_SCENE_HPP

#include "IRigidBody.hpp"
#include "IConstraint.hpp"

class IPhysicsScene
{
public:
	virtual ~IPhysicsScene() {}

public:
	virtual void Update(float deltaTime) = 0;
	virtual void RegisterBody  (IRigidBody* ibody) = 0;
	virtual void UnregisterBody(IRigidBody* ibody) = 0;
	virtual void RegisterConstraint  (IConstraint* constraint) = 0;
	virtual void UnregisterConstraint(IConstraint* constraint) = 0;
};




#endif // !PHYSICS_SCENE_HPP
