#pragma once
#ifndef ICAMERA_MANAGER_HPP
#define ICAMERA_MANAGER_HPP

#include "ICameraComponent.hpp"


struct ICamaeraManager
{
	~ICamaeraManager() {}

	virtual void UnregisterCamera(ICameraComponent* camera) = 0;
	virtual void RegisterCamera	 (ICameraComponent* camera, bool bActivate = true) = 0;
	virtual void SetCameraActive (ICameraComponent* camera, bool newState  = true) = 0;
	
	virtual       ICameraComponent* GetActiveCamera()       = 0;
	virtual const ICameraComponent* GetActiveCamera() const = 0;
};


#endif // !ICAMERA_MANAGER_HPP
