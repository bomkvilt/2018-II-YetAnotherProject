#include "JointComponent.hpp"
#include "Injection/DependencyInjectionManager.hpp"


JointComponent::JointComponent()
{
	constraint = DependencyInjectionManager::MakeConstraint(this);
}

FConstraintType JointComponent::GetConstraint()
{
	return constraintType;
}

void JointComponent::SetConstraint(FConstraintType type)
{
	constraintType = type;
	if (constraint)
	{
		constraint->SetContraints(type);
	}
}

void JointComponent::OnSubcomponentAttached(BaseActorComponent* child)
{
	if (constraint && child && child->GetRigidBody())
	{
		constraint->AddChild(child->GetRigidBody());
	}
}

void JointComponent::OnSubcomponentDetached(BaseActorComponent* child)
{
	if (constraint && child && child->GetRigidBody())
	{
		constraint->RemoveChild(child->GetRigidBody());
	}
}

void JointComponent::OnComponentAttached(BaseActorComponent* parent)
{
	if (constraint && parent && parent->GetRigidBody())
	{
		constraint->SetParent(parent->GetRigidBody());
	}
}

void JointComponent::OnComponentDetached(BaseActorComponent* parent)
{
	if (constraint && parent && parent->GetRigidBody())
	{
		constraint->RemoveParent(parent->GetRigidBody());
	}
}

void JointComponent::onSubcomponentMoved(BaseActorComponent* child, bool bUpdateBody)
{
	if (constraint && bUpdateBody && child && child->GetRigidBody())
	{
		constraint->UpdateChild(child->GetRigidBody());
	}
}

void JointComponent::onComponentMoved(bool bUpdateBody)
{
	if (constraint && bUpdateBody)
	{
		constraint->UpdateConstraint();
	}
}