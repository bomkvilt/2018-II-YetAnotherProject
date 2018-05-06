#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include <memory>
#include <iostream>

#include "BasePlayerController.hpp"
#include "Camera/CameraManager.hpp"
#include "Control/EventManager.hpp"
#include "Control/EventCollector.hpp"

#include "Interfaces/IViewer.hpp"

class CameraComponent;



/** The class controls an assigned avatars
 */
class PlayerController : public BasePlayerController
{
public:
	using Super = BasePlayerController;
public:

	PlayerController();
	virtual ~PlayerController();

public:

	virtual void OnBeginPlay() override;


	virtual void Tick(float DeltaTime, ETickType type);
	virtual void Render();

public: //~~~~~~~~~~~~~~~~~~~| Camera

	void UnregisterCamera(CameraComponent* camera);
	void RegisterCamera	 (CameraComponent* camera, bool bActivate = true);
	void SetCameraActive (CameraComponent* camera, bool newState  = true);
	CameraComponent* GetActiveCamera();
	CameraManager&   GetCameraManager();

public: //~~~~~~~~~~~~~~~~~~~| Events

	EventCollector& GetEventCollector();

protected:

	void processInput();

protected:

	/// >> camera
	CameraManager cameraManager;
	/// <<

	/// >> control
	EventCollector eventCollector;
	EventManager   eventManager;
	/// <<

	/// >> window
	UNIQUE(IViewer) viewer;
	///

};

#endif // !PLAYER_CONTROLLER_HPP