#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

#include <deque>
#include <vector>

class CameraComponent;


/** Module of PlayerController handling in-game cameras 
 *	end place a osg cam into location of an active in-game one
 * 
 *	. contains an array of registered cameras in order of placement(the lates in tail)
 *	. contains a pointer to currently active one
 *	. on update event sync place a scene camera to required location
 *	. in case of the camera is removed the lates becomes an active one
 */
class CameraManager
{
public:
	CameraManager();

	void UnregisterCamera(CameraComponent* camera);
	void RegisterCamera	 (CameraComponent* camera, bool bActivate = true);
	void SetCameraActive (CameraComponent* camera, bool newState  = true);

		  CameraComponent* GetCurrentCamera();
	const CameraComponent* GetCurrentCamera() const;

protected:
	int  FindCamera	(CameraComponent* camera);
	int  PlaceCamera(CameraComponent* camera);

	void ActivateNext(int currentPos);
	void RemoveCamera(int currentPos);

protected:
	/// >> cameras
	int activeCamera;
	std::deque<CameraComponent*> cameras;
	/// <<
};


#endif // ! CAMERA_CONTROLLER_HPP
