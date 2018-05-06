#include "Constraint.hpp"

FConstraint::FConstraint(ActorComponent* owner)
	: owner(owner)
{}

FConstraintType FConstraint::GetConstraints() const
{
	return constraint;
}

World * FConstraint::GetWorld()
{
	if (owner)
	{
		return owner->GetWorld();
	}
	return nullptr;
}
