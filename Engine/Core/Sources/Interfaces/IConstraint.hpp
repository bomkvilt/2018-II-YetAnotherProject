#pragma once
#ifndef ICONSTRAINT_HPP
#define ICONSTRAINT_HPP

#include "Common.hpp"
#include "IRigidBody.hpp"

struct IConstraint
{
public:
	virtual ~IConstraint() {}

	virtual void AddChild    (IRigidBody* ibody) = 0;
	virtual void SetParent   (IRigidBody* ibody) = 0;

	virtual void RemoveChild (IRigidBody* ibody) = 0;
	virtual void RemoveParent(IRigidBody* ibody) = 0;

	virtual void UpdateConstraint() = 0;             // constraint's local transform changed
	virtual void UpdateChild(IRigidBody* ibody) = 0; // subcomponents's one changed

public:
	virtual FConstraintType GetConstraints() const = 0;
	virtual void SetContraints(FConstraintType& newConstraints) = 0;
};


#endif // !ICONSTRAINT_HPP
