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

b2Joint* Constraint::GetConstraint(RigidBody* child)
{
	return childConstraints[child];
}

b2Joint* Constraint::MakeConstraint(RigidBody* parent, RigidBody* child)
{
	auto bRot = constraint.rotation[eZ].bAcive;
	auto bX   = constraint.movement[eX].bAcive;
	auto bY   = constraint.movement[eY].bAcive;

	if (auto* scene = GetPhysicsScene<PhysicsScene>())
	if (parent && parent->rigidBody && child && child->rigidBody)
	if (bRot && !bX && !bY)
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
		return scene->world.CreateJoint(&def);
	}
	else if (!bRot && bX && !bY)
	{
		std::cout << "X" << std::endl;
		b2PrismaticJointDef def;
		def.bodyA = parent->rigidBody;
		def.bodyB = child ->rigidBody;
		def.localAnchorA = this ->GetTransform().p;
		def.localAnchorB = child->GetTransform().p;

		def.enableMotor      = false;
		def.collideConnected = true;

		def.localAxisA       = b2Vec2(1, 0);
		def.enableLimit      = constraint.movement[eX].bAcive;
		def.lowerTranslation = constraint.movement[eX].min;
		def.upperTranslation = constraint.movement[eX].max;
		return scene->world.CreateJoint(&def);
	}
	else if (!bRot && !bX && bY)
	{
		std::cout << "Y" << std::endl;
		b2PrismaticJointDef def;
		def.bodyA = parent->rigidBody;
		def.bodyB = child ->rigidBody;
		def.localAnchorA = this ->GetTransform().p;
		def.localAnchorB = child->GetTransform().p;
		def.enableMotor      = false;
		def.collideConnected = true;

		def.localAxisA       = b2Vec2(0, 1);
		def.enableLimit      = constraint.movement[eY].bAcive;
		def.lowerTranslation = constraint.movement[eY].min;
		def.upperTranslation = constraint.movement[eY].max;
		return  scene->world.CreateJoint(&def);
	}
	else throw std::runtime_error("Unsupported constraint type");
	
	return nullptr;
}
