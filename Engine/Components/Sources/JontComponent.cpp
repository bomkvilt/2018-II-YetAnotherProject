#include "JontComponent.hpp"
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

void JointComponent::OnSubcomponentAttached(ActorComponent* child)
{
	if (constraint && child && child->GetRigidBody())
	{
		constraint->AddChild(child->GetRigidBody());
	}
}

void JointComponent::OnSubcomponentDetached(ActorComponent* child)
{
	if (constraint && child && child->GetRigidBody())
	{
		constraint->RemoveChild(child->GetRigidBody());
	}
}

void JointComponent::OnComponentAttached(ActorComponent* parent)
{
	if (constraint && parent && parent->GetRigidBody())
	{
		constraint->SetParent(parent->GetRigidBody());
	}
}

void JointComponent::OnComponentDetached(ActorComponent* parent)
{
	if (constraint && parent && parent->GetRigidBody())
	{
		constraint->RemoveParent(parent->GetRigidBody());
	}
}

void JointComponent::onSubcomponentMoved(ActorComponent* child, bool bUpdateBody)
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