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
	bool bWorld = space == ESpaceType::eWorld;
	bool bLocal = space == ESpaceType::eLocal;

	Transform local = relativeTarnsform;
	Transform world = worldTransform;
	
	Transform delta_P = parent ? parent->GetComponentTransform() : Transform::getIdentity();
	Transform delta_W = bWorld ? delta : delta_P			 * delta;
	Transform delta_R = bLocal ? delta : delta_P.inverse() * delta;

	relativeTarnsform = delta_R * local;
	worldTransform    = delta_W * world;

	UpdateFacade();

	for (auto* child : subcomponents)
	{
		if (IsValid(child))
		{
			child->UpdateWoldTransform();
		}
	}
}

void ActorComponent::AddComponentLocation(Vector3f delta, ESpaceType space)
{
	AddTransform(Transform(Quatf::getIdentity(), delta), space);
}

void ActorComponent::AddComponentRotation(Quatf delta, ESpaceType space)
{
	AddTransform(Transform(delta, Vector3f(0,0,0)), space);
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
		parent = nullptr;

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
	Transform P = parent ? parent->GetComponentTransform() : Transform::getIdentity();
	Transform L = GetRelativeTransform();

	worldTransform = P * L;

	UpdateFacade();

	for (auto child : subcomponents)
	{
		if (IsValid(child))
		{
			child->UpdateWoldTransform();
		}
	}
}
