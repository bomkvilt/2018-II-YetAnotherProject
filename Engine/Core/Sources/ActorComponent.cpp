#include "ActorComponent.hpp"

#include "Avatar.hpp"
#include "Injection/DependencyInjectionManager.hpp"

ActorComponent::ActorComponent()
	: owner(ThreadContext::TopInitialiser()->owner)
	, parent(nullptr)
	
	, OnCollisionEnter(world)
	, OnCollisionExit (world)

	, relativeTarnsform(FTransform::Identity)
	, worldTransform   (FTransform::Identity)
	
	, facade(DependencyInjectionManager::MakeFacade(this))
	, rigidBody(nullptr)
{}

void ActorComponent::AddForce          (const FVector& force  , ESpaceType space) { if (rigidBody) rigidBody->AddForce          (SpaceToWorld(force  , space)); }
void ActorComponent::AddTorque         (const FVector& torque , ESpaceType space) { if (rigidBody) rigidBody->AddTorque         (SpaceToWorld(torque , space)); }
void ActorComponent::AddImpulce        (const FVector& impulce, ESpaceType space) { if (rigidBody) rigidBody->AddImpulce        (SpaceToWorld(impulce, space)); }
void ActorComponent::AddKineticMomement(const FVector& moment , ESpaceType space) { if (rigidBody) rigidBody->AddKineticMomement(SpaceToWorld(moment , space)); }

FVector ActorComponent::GetVelocity() const { if (rigidBody) return rigidBody->GetVelocity(); return FVector::ZeroVector; }
FVector ActorComponent::GetOmega   () const { if (rigidBody) return rigidBody->GetOmega   (); return FVector::ZeroVector; }
float   ActorComponent::GetMass    () const { if (rigidBody) return rigidBody->GetMass    (); return 0;                   }
FVector ActorComponent::GetInertia () const { if (rigidBody) return rigidBody->GetInertia (); return FVector::ZeroVector; }

void ActorComponent::SetMass   (float newMass     ) { if (rigidBody) rigidBody->SetMass   (newMass   ); }
void ActorComponent::SetInertia(FVector newInertia) { if (rigidBody) rigidBody->SetInertia(newInertia); }


void ActorComponent::SetComponentTransform(FTransform newTransform, bool bExcludePhysics, bool bUpdateBody)
{
	worldTransform = newTransform;
	UpdateRelativeTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void ActorComponent::SetRelativeTransform(FTransform newTransform, bool bExcludePhysics, bool bUpdateBody)
{
	relativeTarnsform = newTransform;
	UpdateWoldTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void ActorComponent::SetComponentLocation(FVector newLocation, bool bExcludePhysics, bool bUpdateBody)
{
	worldTransform.Location = newLocation;
	UpdateRelativeTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void ActorComponent::SetRelativeLocation(FVector newLocation, bool bExcludePhysics, bool bUpdateBody)
{
	relativeTarnsform.Location = newLocation;
	UpdateWoldTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void ActorComponent::SetComponentRotation(FQuat newRotation, bool bExcludePhysics, bool bUpdateBody)
{
	worldTransform.Rotation = newRotation;
	UpdateRelativeTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void ActorComponent::SetRelativeRotation(FQuat newRotation, bool bExcludePhysics, bool bUpdateBody)
{
	relativeTarnsform.Rotation = newRotation;
	UpdateWoldTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void ActorComponent::AddTransform(FTransform delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody)
{	
	if (space == ESpaceType::eLocal)
	{
		relativeTarnsform += delta;
		UpdateWoldTransform(bExcludePhysics, bUpdateBody);
		onComponentMoved(bUpdateBody);
		return;
	}
	if (space == ESpaceType::eWorld)
	{
		worldTransform += delta;
		UpdateRelativeTransform(bExcludePhysics, bUpdateBody);
		onComponentMoved(bUpdateBody);
		return;
	}
	if (space == ESpaceType::eParent)
	{
		relativeTarnsform = delta * relativeTarnsform;
		UpdateWoldTransform(bExcludePhysics, bUpdateBody);
		onComponentMoved(bUpdateBody);
		return;
	}
	throw std::out_of_range("Space have unsupported value");
}

void ActorComponent::AddComponentLocation(FVector delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody)
{
	AddTransform(FTransform(delta), space, bExcludePhysics, bUpdateBody);
}

void ActorComponent::AddComponentRotation(FQuat delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody)
{
	AddTransform(FTransform(delta), space, bExcludePhysics, bUpdateBody);
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
		UpdateWoldTransform(false,true);
		OnComponentAttached(newParent);

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

		UpdateRelativeTransform(false,true);
		OnComponentDetached(lastParent);
	}
}

bool ActorComponent::IsDynamic() const
{
	if (rigidBody)
	{
		return rigidBody->IsDinamic();
	}
	return false;
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

FVector ActorComponent::SpaceToWorld(const FVector& v, ESpaceType space) const
{
	switch (space) {
	case eParent:   return GetParentTransform().Rotation * v;
	case eLocal:    return worldTransform.Rotation * v;
	case eWorld:    return v;
	}
	throw std::out_of_range("Space have unsupported value");
}

FQuat ActorComponent::SpaceToWorld(const FQuat& v, ESpaceType space) const
{
	switch (space) {
	case eParent:   return GetParentTransform() * v;
	case eLocal:    return worldTransform * v;
	case eWorld:    return v;
	}
	throw std::out_of_range("Space have unsupported value");
}

FVector ActorComponent::SpaceToLocal(const FVector& v, ESpaceType space) const
{
	switch (space) {
	case eParent:   return ~relativeTarnsform.Rotation * v;
	case eWorld:    return ~worldTransform.Rotation * v;
	case eLocal:    return v;
	}
	throw std::out_of_range("Space have unsupported value");
}

FQuat ActorComponent::SpaceToLocal(const FQuat& v, ESpaceType space) const
{
	switch (space) {
	case eParent:   return ~relativeTarnsform * v;
	case eWorld:    return ~worldTransform * v;
	case eLocal:    return v;
	}
	throw std::out_of_range("Space have unsupported value");
}

void ActorComponent::AddSubcomponent(ActorComponent* child)
{
	if (child)
	{
		subcomponents.emplace_back(child);
		OnSubcomponentAttached(child);
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
			OnSubcomponentDetached(child);
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

void ActorComponent::UpdateBody()
{
	if (rigidBody)
	{
		rigidBody->Update();
	}
}

void ActorComponent::UpdateWoldTransform(bool bExcludePhysics, bool bUpdateBody)
{
	FTransform P = GetParentTransform();
	FTransform R = GetRelativeTransform();
	worldTransform = P * R;
	UpdateChainTransforms(bExcludePhysics, bUpdateBody);
}

void ActorComponent::UpdateRelativeTransform(bool bExcludePhysics, bool bUpdateBody)
{
	FTransform P = GetParentTransform();
	FTransform W = GetComponentTransform();
	relativeTarnsform = ~P * W;
	UpdateChainTransforms(bExcludePhysics, bUpdateBody);
}

void ActorComponent::UpdateChainTransforms(bool bExcludePhysics, bool bUpdateBody)
{	
	if (bUpdateBody)
	{
		UpdateBody();
	}
	UpdateFacade();
	
	for (auto child : subcomponents)
	{
		if (IsValid(child) && !(bExcludePhysics && child->IsDynamic()))
		{
			child->UpdateWoldTransform(bExcludePhysics, true);
		}
	}
}

FTransform ActorComponent::GetParentTransform() const
{
	return parent ? parent->GetComponentTransform() : FTransform::Identity;
}

void ActorComponent::onComponentMoved(bool bUpdateBody)
{
	if (parent)
	{
		parent->onSubcomponentMoved(this, bUpdateBody);
	}
}

