#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP
#pragma once

#include <deque>
#include <vector>

#include "Interfaces/ICameraManager.hpp"


/** Module of PlayerController handling in-game cameras 
 *	end place a osg cam into location of an active in-game one
 * 
 *	. contains an array of registered cameras in order of placement(the lates in tail)
 *	. contains a pointer to currently active one
 *	. on update event sync place a scene camera to required location
 *	. in case of the camera is removed the lates becomes an active one
 */
class CameraManager : public ICamaeraManager
{
public:
	CameraManager();

	virtual void UnregisterCamera(ICameraComponent* camera) override;
	virtual void RegisterCamera	 (ICameraComponent* camera, bool bActivate = true) override;
	virtual void SetCameraActive (ICameraComponent* camera, bool newState  = true) override;

	virtual       ICameraComponent* GetActiveCamera()       override;
	virtual const ICameraComponent* GetActiveCamera() const override;

private:
	int FindCamera (ICameraComponent* camera);
	int PlaceCamera(ICameraComponent* camera);

	void ActivateNext(int currentPos);
	void RemoveCamera(int currentPos);

protected:
	/// >> cameras
	int activeCamera;
	std::deque<ICameraComponent*> cameras;
	/// <<
};


#endif // ! CAMERA_CONTROLLER_HPP
