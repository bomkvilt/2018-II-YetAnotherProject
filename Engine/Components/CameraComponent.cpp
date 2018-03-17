#include "CameraComponent.hpp"

#include "../Core/PlayerController.hpp"


CameraComponent::CameraComponent()
	: bActive		(false)
	, bRegistered	(false)
	, bAutoRegister	(true )
{}

void CameraComponent::OnBeginPlay()
{
	if (bAutoRegister && !bRegistered)
	{
		Register(true);
	}
}

void CameraComponent::OnEndPlay()
{
	Register(false);
}

void CameraComponent::SetAutoregister(bool newFlag) 
{ 
	bAutoRegister = newFlag; 

	if (bAutoRegister && !bRegistered)
	{
		Register(true);
	}
}

bool CameraComponent::GetAutoregister() const 
{ 
	return bAutoRegister; 
}

bool CameraComponent::Register(bool newState)
{
	if (auto* controller = GetPlayerController())
	{
		if (newState)
		{
			controller->RegisterCamera(this, bAutoRegister);
			bRegistered = newState;
			bActive = bAutoRegister;
		}
		else
		{
			controller->UnregisterCamera(this);
			bRegistered = newState;
			bActive		= newState;
		}
		return true;
	}	
	bRegistered = false;
	return false;
}

bool CameraComponent::Activate(bool newState)
{
	return false;
}
