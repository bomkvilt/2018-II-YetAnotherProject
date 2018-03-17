#include "ActorComponent.hpp"

#include "Avatar.hpp"


ActorComponent::ActorComponent()
	: owner(ThreadContextSingleton::Get().TopInitialiser()->owner)
	, parent(nullptr)
	, relativeTarnsform(btTransform::getIdentity())
	, worldTransform   (btTransform::getIdentity())
{}

void ActorComponent::AddForce(const btVector3& force, ESpaceType space)
{
	//TODO:
}

void ActorComponent::AddTorque(const btVector3& torue, ESpaceType space)
{
	//TODO:
}

void ActorComponent::SetComponentTransform(btTransform newTransform)
{
	//TODO:
}

void ActorComponent::SetRelativeTransform(btTransform newTransform)
{
	relativeTarnsform = newTransform;
}

void ActorComponent::AddTransform(btTransform delta, ESpaceType space)
{
	bool bWorld = space == ESpaceType::eWorld;
	bool bLocal = space == ESpaceType::eLocal;

	btTransform local = relativeTarnsform;
	btTransform world = worldTransform;
	
	btTransform delta_P = parent ? parent->GetComponentTransform() : btTransform::getIdentity();
	btTransform delta_W = bWorld ? delta : delta_P			 * delta;
	btTransform delta_R = bLocal ? delta : delta_P.inverse() * delta;

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

void ActorComponent::AddComponentLocation(btVector3 delta, ESpaceType space)
{
	AddTransform(btTransform(btQuaternion::getIdentity(), delta), space);
}

void ActorComponent::AddComponentRotation(btQuaternion delta, ESpaceType space)
{
	AddTransform(btTransform(delta, btVector3(0,0,0)), space);
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
	btVector3 newVec = relativeTarnsform.getOrigin();

	osg::Vec3 newPos = osg::Vec3(
		static_cast<float>(newVec.x()),
		static_cast<float>(newVec.y()),
		static_cast<float>(newVec.z())
		);

	facade.GetRoot()->setPosition(newPos);

	btQuaternion newQuat = relativeTarnsform.getRotation();

	osg::Quat newRot = osg::Quat(
		static_cast<float>(newQuat.x()),
		static_cast<float>(newQuat.y()),
		static_cast<float>(newQuat.z()),
		static_cast<float>(newQuat.w())
		);

	facade.GetRoot()->setAttitude(newRot);
}

void ActorComponent::UpdateWoldTransform()
{
	btTransform P = parent ? parent->GetComponentTransform() : btTransform::getIdentity();
	btTransform L = GetRelativeTransform();

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
