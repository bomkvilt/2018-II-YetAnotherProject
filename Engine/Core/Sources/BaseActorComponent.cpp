#include "BaseActorComponent.hpp"

#include "Injection/DependencyInjectionManager.hpp"

BaseActorComponent::BaseActorComponent()
	: owner(ThreadContext::TopInitialiser()->owner)
	, parent(nullptr)
	
	, OnCollisionEnter(world)
	, OnCollisionExit (world)

	, relativeTarnsform(FTransform::Identity)
	, worldTransform   (FTransform::Identity)
	
	
	, rigidBody(nullptr)
{}

void BaseActorComponent::AddForce          (const FVector& force  , ESpaceType space) { if (rigidBody) rigidBody->AddForce          (SpaceToWorld(force  , space)); }
void BaseActorComponent::AddTorque         (const FVector& torque , ESpaceType space) { if (rigidBody) rigidBody->AddTorque         (SpaceToWorld(torque , space)); }
void BaseActorComponent::AddImpulce        (const FVector& impulce, ESpaceType space) { if (rigidBody) rigidBody->AddImpulce        (SpaceToWorld(impulce, space)); }
void BaseActorComponent::AddKineticMomement(const FVector& moment , ESpaceType space) { if (rigidBody) rigidBody->AddKineticMomement(SpaceToWorld(moment , space)); }

FVector BaseActorComponent::GetVelocity() const { if (rigidBody) return rigidBody->GetVelocity(); return FVector::ZeroVector; }
FVector BaseActorComponent::GetOmega   () const { if (rigidBody) return rigidBody->GetOmega()   ; return FVector::ZeroVector; }
float   BaseActorComponent::GetMass    () const { if (rigidBody) return rigidBody->GetMass()    ; return 0;                   }
FVector BaseActorComponent::GetInertia () const { if (rigidBody) return rigidBody->GetInertia (); return FVector::ZeroVector; }

void BaseActorComponent::SetMass   (float newMass     ) { if (rigidBody) rigidBody->SetMass   (newMass   ); }
void BaseActorComponent::SetInertia(FVector newInertia) { if (rigidBody) rigidBody->SetInertia(newInertia); }


void BaseActorComponent::SetComponentTransform(FTransform newTransform, bool bExcludePhysics, bool bUpdateBody)
{
	worldTransform = newTransform;
	UpdateRelativeTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void BaseActorComponent::SetRelativeTransform(FTransform newTransform, bool bExcludePhysics, bool bUpdateBody)
{
	relativeTarnsform = newTransform;
	UpdateWoldTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void BaseActorComponent::SetComponentLocation(FVector newLocation, bool bExcludePhysics, bool bUpdateBody)
{
	worldTransform.Location = newLocation;
	UpdateRelativeTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void BaseActorComponent::SetRelativeLocation(FVector newLocation, bool bExcludePhysics, bool bUpdateBody)
{
	relativeTarnsform.Location = newLocation;
	UpdateWoldTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void BaseActorComponent::SetComponentRotation(FQuat newRotation, bool bExcludePhysics, bool bUpdateBody)
{
	worldTransform.Rotation = newRotation;
	UpdateRelativeTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void BaseActorComponent::SetRelativeRotation(FQuat newRotation, bool bExcludePhysics, bool bUpdateBody)
{
	relativeTarnsform.Rotation = newRotation;
	UpdateWoldTransform(bExcludePhysics, bUpdateBody);
	onComponentMoved(bUpdateBody);
}

void BaseActorComponent::AddTransform(FTransform delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody)
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

void BaseActorComponent::AddComponentLocation(FVector delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody)
{
	AddTransform(FTransform(delta), space, bExcludePhysics, bUpdateBody);
}

void BaseActorComponent::AddComponentRotation(FQuat delta, ESpaceType space, bool bExcludePhysics, bool bUpdateBody)
{
	AddTransform(FTransform(delta), space, bExcludePhysics, bUpdateBody);
}

void BaseActorComponent::AttachTo(BaseActorComponent* newParent)
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

void BaseActorComponent::Detach()
{
	if (parent)
	{
		auto* lastParent = parent;

		parent->RemoveSubcomponent(this);
		parent = world->GetSceneRoot();

		if (facade)
		{
			facade->Detach();
		}

		UpdateRelativeTransform(false,true);
		OnComponentDetached(lastParent);
	}
}

bool BaseActorComponent::IsDynamic() const
{
	if (rigidBody)
	{
		return rigidBody->IsDinamic();
	}
	return false;
}

std::vector<BaseActorComponent*>& BaseActorComponent::GetSubcomponents()
{
	return subcomponents;
	// std::vector<BaseActorComponent*> components;
	// Internal_GetSubcomponents(components);
	// return components;
}

const std::vector<BaseActorComponent*>& BaseActorComponent::GetSubcomponents() const
{
	return subcomponents;
	// std::vector<const BaseActorComponent*> components;
	// Internal_GetSubcomponents(components);
	// return components;
}

FVector BaseActorComponent::SpaceToWorld(const FVector& v, ESpaceType space) const
{
	switch (space) {
	case eParent:   return GetParentTransform().Rotation * v;
	case eLocal:    return worldTransform.Rotation * v;
	case eWorld:    return v;
	}
	throw std::out_of_range("Space have unsupported value");
}

FQuat BaseActorComponent::SpaceToWorld(const FQuat& v, ESpaceType space) const
{
	switch (space) {
	case eParent:   return GetParentTransform() * v;
	case eLocal:    return worldTransform * v;
	case eWorld:    return v;
	}
	throw std::out_of_range("Space have unsupported value");
}

FVector BaseActorComponent::SpaceToLocal(const FVector& v, ESpaceType space) const
{
	switch (space) {
	case eParent:   return ~relativeTarnsform.Rotation * v;
	case eWorld:    return ~worldTransform.Rotation * v;
	case eLocal:    return v;
	}
	throw std::out_of_range("Space have unsupported value");
}

FQuat BaseActorComponent::SpaceToLocal(const FQuat& v, ESpaceType space) const
{
	switch (space) {
	case eParent:   return ~relativeTarnsform * v;
	case eWorld:    return ~worldTransform * v;
	case eLocal:    return v;
	}
	throw std::out_of_range("Space have unsupported value");
}

void BaseActorComponent::AddSubcomponent(BaseActorComponent* child)
{
	if (child)
	{
		subcomponents.emplace_back(child);
		OnSubcomponentAttached(child);
	}
}

void BaseActorComponent::RemoveSubcomponent(BaseActorComponent* child)
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

void BaseActorComponent::Internal_GetSubcomponents(std::vector<BaseActorComponent*>& components)
{
	for (auto comp : subcomponents)
	{
		if (!IsValid(comp))			continue;
		if (comp->owner != owner)	continue;

		components.emplace_back(comp);

		comp->Internal_GetSubcomponents(components);
	}
}

void BaseActorComponent::Internal_GetSubcomponents(std::vector<const BaseActorComponent*>& components) const
{
	for (auto comp : subcomponents)
	{
		if (!IsValid(comp))			continue;
		if (comp->owner != owner)	continue;

		components.emplace_back(comp);

		comp->Internal_GetSubcomponents(components);
	}
}

void BaseActorComponent::UpdateFacade()
{
	if (facade)
	{
		facade->Update();
	}
}

void BaseActorComponent::UpdateBody()
{
	if (rigidBody)
	{
		rigidBody->Update();
	}
}

void BaseActorComponent::UpdateWoldTransform(bool bExcludePhysics, bool bUpdateBody)
{
	FTransform P = GetParentTransform();
	FTransform R = GetRelativeTransform();
	worldTransform = P * R;
	UpdateChainTransforms(bExcludePhysics, bUpdateBody);
}

void BaseActorComponent::UpdateRelativeTransform(bool bExcludePhysics, bool bUpdateBody)
{
	FTransform P = GetParentTransform();
	FTransform W = GetComponentTransform();
	relativeTarnsform = ~P * W;
	UpdateChainTransforms(bExcludePhysics, bUpdateBody);
}

void BaseActorComponent::UpdateChainTransforms(bool bExcludePhysics, bool bUpdateBody)
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

FTransform BaseActorComponent::GetParentTransform() const
{
	return parent ? parent->GetComponentTransform() : FTransform::Identity;
}

void BaseActorComponent::onComponentMoved(bool bUpdateBody)
{
	if (parent)
	{
		parent->onSubcomponentMoved(this, bUpdateBody);
	}
}

