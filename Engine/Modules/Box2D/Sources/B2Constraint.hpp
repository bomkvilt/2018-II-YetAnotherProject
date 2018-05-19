#pragma once
#ifndef B2CONSTRAINT_HPP
#define B2CONSTRAINT_HPP

#include <unordered_map>
#include "Box2D/Box2D.h"

#include "Interfaces/Default/Constraint.hpp"
#include "B2RigidBody.hpp"
#include "B2Convertion.hpp"

class Constraint : public FConstraint
{
public: 
	Constraint(BaseActorComponent* owner);

public:
	virtual void AddChild    (IRigidBody* ibody) override;
	virtual void SetParent   (IRigidBody* ibody) override;

	virtual void RemoveChild (IRigidBody* ibody) override;
	virtual void RemoveParent(IRigidBody* ibody) override;

	virtual void UpdateConstraint() override;            
	virtual void UpdateChild(IRigidBody* ibody) override;
	
	virtual void SetContraints(FConstraintType& newConstraints) override;

private:
	b2Transform GetTransform() const;
	b2Joint* GetConstraint(RigidBody* child);
	b2Joint* MakeConstraint(RigidBody* parent, RigidBody* child);

private:
	RigidBody* parent;
	std::unordered_map<RigidBody*, b2Joint*> childConstraints; //TODO:: make a mare generic joint
};



#endif // !B2CONSTRAINT_HPP
