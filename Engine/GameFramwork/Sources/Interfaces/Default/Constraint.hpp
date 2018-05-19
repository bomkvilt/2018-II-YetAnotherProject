#pragma once
#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include "Interfaces/IConstraint.hpp"
#include "BaseActorComponent.hpp"


struct FConstraint : public IConstraint
{
	FConstraint(BaseActorComponent* owner);

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
	BaseActorComponent* owner;
	FConstraintType constraint;
};



#endif //!CONSTRAINT_HPP