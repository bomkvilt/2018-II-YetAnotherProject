#ifndef B2SCENE_HPP
#define B2SCENE_HPP
#pragma once

#include <set>
#include <unordered_map>
#include "Box2D/Box2D.h"

#include "B2RigidBody.hpp"
#include "Interfaces/IPhysicsScene.hpp"
#include "Collision/ContactListener.hpp"


class PhysicsScene : public IPhysicsScene
{
public:
	PhysicsScene();

public:
	virtual void Update(float deltaTime) override;
	virtual void RegisterBody  (IRigidBody* ibody) override;
	virtual void UnregisterBody(IRigidBody* ibody) override;
	virtual void RegisterConstraint  (IConstraint* constraint) override {}
	virtual void UnregisterConstraint(IConstraint* constraint) override {}

public:
	b2World world;

	ContactListener contactListener;
	std::set<RigidBody*> rigidBodies;
	std::unordered_map<b2Body*, RigidBody*> body2body;

private:
	void SyncRigidBodies();
	void ProcessCollisions();
};



#endif // !B2SCENE_HPP
