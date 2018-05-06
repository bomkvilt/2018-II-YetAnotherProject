#include "ActorComponent.hpp"

#include "Avatar.hpp"
#include "Injection/DependencyInjectionManager.hpp"

ActorComponent::ActorComponent()
	: owner(ThreadContext::TopInitialiser()->owner)
	, parent(nullptr)

	, relativeTarnsform(FTransform::Identity)
	, worldTransform   (FTransform::Identity)
	
	, facade(DependencyInjectionManager::MakeFacade(this))
{}

void ActorComponent::AddForce(const FVector& force, ESpaceType space)
{
	//TODO:
}

void ActorComponent::AddTorque(const FVector& torue, ESpaceType space)
{
	//TODO:
}

void ActorComponent::SetComponentTransform(FTransform newTransform)
{
	worldTransform = newTransform;
	UpdateRelativeTransform();
}

void ActorComponent::SetRelativeTransform(FTransform newTransform)
{
	relativeTarnsform = newTransform;
	UpdateWoldTransform();
}

void ActorComponent::SetComponentLocation(FVector newLocation)
{
	worldTransform.Location = newLocation;
	UpdateRelativeTransform();
}

void ActorComponent::SetRelativeLocation(FVector newLocation)
{
	relativeTarnsform.Location = newLocation;
	UpdateWoldTransform();
}

void ActorComponent::SetComponentRotation(FQuat newRotation)
{
	worldTransform.Rotation = newRotation;
	UpdateRelativeTransform();
}

void ActorComponent::SetRelativeRotation(FQuat newRotation)
{
	relativeTarnsform.Rotation = newRotation;
	UpdateWoldTransform();
}

void ActorComponent::AddTransform(FTransform delta, ESpaceType space)
{	
	if (space == ESpaceType::eLocal)
	{
		relativeTarnsform += delta;
		UpdateWoldTransform();
		return;
	}
	if (space == ESpaceType::eWorld)
	{
		worldTransform += delta;
		UpdateRelativeTransform();
		return;
	}
	if (space == ESpaceType::eParent)
	{
		relativeTarnsform = delta * relativeTarnsform;
		UpdateWoldTransform();
		return;
	}
	throw std::out_of_range("Space have unsupported value");
}

void ActorComponent::AddComponentLocation(FVector delta, ESpaceType space)
{
	AddTransform(FTransform(delta), space);
}

void ActorComponent::AddComponentRotation(FQuat delta, ESpaceType space)
{
	AddTransform(FTransform(delta), space);
}

const PlayerController* ActorComponent::GetPlayerController() const
{
	auto*  avatar = dynamic_cast<Avatar*>(owner);
	return avatar ? avatar->GetPlayerController() : nullptr;
}

PlayerController* ActorComponent::GetPlayerController()
{
	auto*  avatar = dynamic_cast<Avatar*>(owner);
	return avatar ? avatar->GetPlayerController() : nullptr;
}

void ActorComponent::AttachTo(ActorComponent* newParent)
{
	if (world) 
	{
		Detach();

		if (!newParent) 
		{
			newParent = world->GetSceneRoot();
		}
		parent = newParent;
		parent->AddSubcomponent(this);
		UpdateWoldTransform();

		if (facade)
		{
			facade->AttachTo(newParent->GetFacade());
		}
	}
}

void ActorComponent::Detach()
{
	if (parent && world)
	{
		auto* lastParent = parent;

		parent->RemoveSubcomponent(this);
		parent = world->GetSceneRoot();
		facade->Detach();

		UpdateRelativeTransform();
	}
}

std::vector<ActorComponent*>& ActorComponent::GetSubcomponents()
{
	return subcomponents;
	// std::vector<ActorComponent*> components;
	// Internal_GetSubcomponents(components);
	// return components;
}

const std::vector<ActorComponent*>& ActorComponent::GetSubcomponents() const
{
	return subcomponents;
	// std::vector<const ActorComponent*> components;
	// Internal_GetSubcomponents(components);
	// return components;
}

void ActorComponent::AddSubcomponent(ActorComponent* child)
{
	if (child)
	{
		subcomponents.emplace_back(child);
	}
}

void ActorComponent::RemoveSubcomponent(ActorComponent* child)
{
	if (child)
	{
		auto bgn = subcomponents.begin();
		auto end = subcomponents.end();
		auto pos = std::find(bgn, end, this);
		if (pos != end)
		{
			subcomponents.erase(pos);
		}
	}
}

void ActorComponent::Internal_GetSubcomponents(std::vector<ActorComponent*>& components)
{
	for (auto comp : subcomponents)
	{
		if (!IsValid(comp))			continue;
		if (comp->owner != owner)	continue;

		components.emplace_back(comp);

		comp->Internal_GetSubcomponents(components);
	}
}

void ActorComponent::Internal_GetSubcomponents(std::vector<const ActorComponent*>& components) const
{
	for (auto comp : subcomponents)
	{
		if (!IsValid(comp))			continue;
		if (comp->owner != owner)	continue;

		components.emplace_back(comp);

		comp->Internal_GetSubcomponents(components);
	}
}

void ActorComponent::UpdateFacade()
{
	if (facade)
	{
		facade->Update();
	}
}

void ActorComponent::UpdateWoldTransform()
{
	FTransform P = GetParentTransform();
	FTransform R = GetRelativeTransform();
	worldTransform = P * R;
	UpdateChainTransforms();
}

void ActorComponent::UpdateRelativeTransform()
{
	FTransform P = GetParentTransform();
	FTransform W = GetComponentTransform();
	relativeTarnsform = ~P * W;
	UpdateChainTransforms();
}

void ActorComponent::UpdateChainTransforms()
{
	UpdateFacade();
	
	for (auto child : subcomponents)
	{
		if (IsValid(child))
		{
			child->UpdateWoldTransform();
		}
	}
}

FTransform ActorComponent::GetParentTransform() const
{
	return parent ? parent->GetComponentTransform() : FTransform::Identity;
}
