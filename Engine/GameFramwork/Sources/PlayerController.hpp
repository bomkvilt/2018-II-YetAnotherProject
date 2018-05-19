#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP
#pragma once

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
	, public ICameraManager
{
	GENERATED_BODY(PlayerController, BasePlayerController)

public:
	PlayerController();

public:
	virtual void OnBeginPlay() override;

	virtual void Tick(float DeltaTime, ETickType type) override;
	virtual void Render();

public: //~~~~~~~~~~~~~~~~~~~| Camera

	virtual void UnregisterCamera(ICameraComponent* camera) override;
	virtual void RegisterCamera	 (ICameraComponent* camera, bool bActivate = true) override;
	virtual void SetCameraActive (ICameraComponent* camera, bool newState  = true) override;
	
	virtual       ICameraComponent* GetActiveCamera()       override;
	virtual const ICameraComponent* GetActiveCamera() const override;

	CameraManager& GetCameraManager();

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