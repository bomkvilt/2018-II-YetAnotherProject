#include "B2Constraint.hpp"
#include "B2PhysicsScene.hpp"
#include <iostream>

Constraint::Constraint(BaseActorComponent* owner)
	: FConstraint(owner)
{}

void Constraint::AddChild(IRigidBody* ibody)
{
	auto* body = dynamic_cast<RigidBody*>(ibody);
	
	if (auto* joint = MakeConstraint(parent, body))
	{
		childConstraints.emplace(body, joint);
	}
}

void Constraint::SetParent(IRigidBody* ibody)
{
	if (auto* newParent = dynamic_cast<RigidBody*>(ibody))
	{
		parent = newParent;
		UpdateConstraint();
	}
}

void Constraint::RemoveChild(IRigidBody* ibody)
{
	if (auto* body = dynamic_cast<RigidBody*>(ibody))
	{
		if (auto* scene = GetPhysicsScene<PhysicsScene>())
		{
			scene->world.DestroyJoint(GetConstraint(body));
		}
		childConstraints.erase(body);
	}
}

void Constraint::RemoveParent(IRigidBody* ibody)
{
	if (auto* scene = GetPhysicsScene<PhysicsScene>())
	{
		for (auto& pair : childConstraints)
		{
			if (auto& joint = pair.second)
			{
				scene->world.DestroyJoint(joint);
				pair.second = nullptr;
			}
		}
		parent = nullptr;
	}
}

void Constraint::UpdateConstraint()
{
	for (auto& pair : childConstraints)
	{
		UpdateChild(pair.first);
	}
}

void Constraint::UpdateChild(IRigidBody* ibody)
{
	if (auto* scene = GetPhysicsScene<PhysicsScene>())
	if (auto* body  = dynamic_cast<RigidBody*>(ibody))
	if (auto& joint = childConstraints[body])
	{
		scene->world.DestroyJoint(joint);
		joint = MakeConstraint(parent, body);
	}
	else
	{
		joint = MakeConstraint(parent, body);
	}
}

void Constraint::SetContraints(FConstraintType& newConstraints)
{
	constraint = newConstraints;
	UpdateConstraint();
}

b2Transform Constraint::GetTransform() const
{
	if (owner)
	{
		return b2Transform() << owner->GetRelativeTransform();
	}
	return b2Transform();
}

b2RevoluteJoint* Constraint::GetConstraint(RigidBody* child)
{
	return childConstraints[child];
}

b2RevoluteJoint* Constraint::MakeConstraint(RigidBody* parent, RigidBody* child)
{
	if (auto* scene = GetPhysicsScene<PhysicsScene>())
	if (parent && parent->rigidBody && child && child->rigidBody)
	{
		b2RevoluteJointDef def;
		def.bodyA = parent->rigidBody;
		def.bodyB = child ->rigidBody;
		def.localAnchorA = this ->GetTransform().p;
		def.localAnchorB = child->GetTransform().p;

		def.enableMotor      = false;
		def.collideConnected = true;
		
		def.enableLimit = constraint.rotation[eZ].bAcive;
		def.lowerAngle  = DEG2RAD(constraint.rotation[eZ].min);
		def.upperAngle  = DEG2RAD(constraint.rotation[eZ].max);
		(b2RevoluteJoint*)scene->world.CreateJoint(&def);
	}
	return nullptr;
}
