#pragma once
#ifndef JOINT_COMPONENT_HPP
#define JOINT_COMPONENT_HPP

#include "ActorComponent.hpp"
#include "Interfaces/IConstraint.hpp"


class JointComponent : public ActorComponent
{
	GENERATED_BODY(JointComponent, BaseActorComponent);

public:
	JointComponent();

public:
	FConstraintType GetConstraint();
	void SetConstraint(FConstraintType type);

protected:
	virtual void OnSubcomponentAttached(BaseActorComponent* child ) override;
	virtual void OnSubcomponentDetached(BaseActorComponent* child ) override;
	virtual void OnComponentAttached   (BaseActorComponent* parent) override;
	virtual void OnComponentDetached   (BaseActorComponent* parent) override;
	virtual void onSubcomponentMoved   (BaseActorComponent* child, bool bUpdateBody) override;
	virtual void onComponentMoved      (bool bUpdateBody) override;

protected:
	UNIQUE(IConstraint) constraint;
	FConstraintType constraintType;	
};


#endif // !JOINT_COMPONENT_HPP
