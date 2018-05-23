#include "ActorComponent.hpp"
#include "Avatar.hpp"


PlayerController* ActorComponent::GetPlayerController()
{
	if (auto* avatar = dynamic_cast<Avatar*>(GetOwner()))
	{
		return avatar->GetPlayerController();
	}
	return nullptr;
}

const PlayerController* ActorComponent::GetPlayerController() const
{
	if (auto* avatar = dynamic_cast<const Avatar*>(GetOwner()))
	{
		return avatar->GetPlayerController();
	}
	return nullptr;
}
