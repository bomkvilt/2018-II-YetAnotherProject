#include "ActorComponent.hpp"

#include "Avatar.hpp"


ActorComponent::ActorComponent()
	: owner(ThreadContextSingleton::Get().TopInitialiser()->owner)
	, parent(nullptr)
	, relativeTarnsform(Transform::getIdentity())
	, worldTransform   (Transform::getIdentity())
{}

void ActorComponent::AddForce(const Vector3f& force, ESpaceType space)
{
	//TODO:
}

void ActorComponent::AddTorque(const Vector3f& torue, ESpaceType space)
{
	//TODO:
}

void ActorComponent::SetComponentTransform(Transform newTransform)
{
	//TODO:
}

void ActorComponent::SetRelativeTransform(Transform newTransform)
{
	relativeTarnsform = newTransform;
}

void ActorComponent::AddTransform(Transform delta, ESpaceType space)
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
	assert(false);
}

void ActorComponent::AddComponentLocation(Vector3f delta, ESpaceType space)
{
	AddTransform(Transform(delta), space);
}

void ActorComponent::AddComponentRotation(Quatf delta, ESpaceType space)
{
	AddTransform(Transform(delta), space);
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
	if (!world) return;

	if (!newParent) {
		newParent = world->GetSceneRoot();
	}
	Detach();

	parent = newParent;
	parent->subcomponents.emplace_back(this);

	facade.AttachTo(newParent->facade);
}

void ActorComponent::Detach()
{
	if (parent)	
	{
		auto pos = parent->subcomponents.begin();
		auto end = parent->subcomponents.end();

		for (; pos != end; ++pos)
		{
			if (*pos == this)
			{
				parent->subcomponents.erase(pos);
				break;
			}
		}
		parent = world->GetSceneRoot(); //TODO

		facade.Detach();
	}
}

std::vector<ActorComponent*> ActorComponent::GetSubcomponents()
{
	std::vector<ActorComponent*> components;
	Internal_GetSubcomponents(components);
	return components;
}

std::vector<const ActorComponent*> ActorComponent::GetSubcomponents() const
{
	std::vector<const ActorComponent*> components;
	Internal_GetSubcomponents(components);
	return components;
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
	Vector3f newVec = relativeTarnsform.getOrigin();

	osg::Vec3 newPos = osg::Vec3(
		static_cast<float>(newVec.x),
		static_cast<float>(newVec.y),
		static_cast<float>(newVec.z)
		);

	facade.GetRoot()->setPosition(newPos);
	
	Quatf newQuat = relativeTarnsform.getRotation();

	osg::Quat newRot = osg::Quat(
		static_cast<float>(newQuat.v.x),
		static_cast<float>(newQuat.v.y),
		static_cast<float>(newQuat.v.z),
		static_cast<float>(newQuat.w)
		);

	facade.GetRoot()->setAttitude(newRot);
}

void ActorComponent::UpdateWoldTransform()
{
	Transform P = GetParentTransform();
	Transform R = GetRelativeTransform();
	worldTransform = P * R;
	UpdateChainTransforms();
}

void ActorComponent::UpdateRelativeTransform()
{
	Transform P = GetParentTransform();
	Transform W = GetComponentTransform();
	relativeTarnsform = ~P * W;
	UpdateChainTransforms();
}

void ActorComponent::UpdateChainTransforms()
{
	UpdateFacade();

	std::cout << name << std::endl;
	std::cout << worldTransform   .origin << " " << worldTransform   .rotation << std::endl;
	std::cout << relativeTarnsform.origin << " " << relativeTarnsform.rotation << std::endl;

	for (auto child : subcomponents)
	{
		if (IsValid(child))
		{
			child->UpdateWoldTransform();
		}
	}
}

Transform ActorComponent::GetParentTransform() const
{
	return parent ? parent->GetComponentTransform() : Transform::getIdentity();
}
