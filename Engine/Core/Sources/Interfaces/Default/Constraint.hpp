#pragma once
#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include "../IConstraint.hpp"
#include "ActorComponent.hpp"


struct FConstraint : public IConstraint
{
	FConstraint(ActorComponent* owner);

	virtual FConstraintType GetConstraints() const override;

protected:

	template<class _ST> 
	_ST* GetPhysicsScene()
	{
		if (auto* world = GetWorld())
		{
			return dynamic_cast<_ST*>(world->GetPhysicsScene());
		}
		return nullptr;
	}

	World* GetWorld();

protected:
	ActorComponent* owner;
	FConstraintType constraint;
};



#endif //!CONSTRAINT_HPP