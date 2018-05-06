#pragma once
#ifndef VIEWER_HPP
#define VIEWER_HPP

#include "Interfaces/IViewer.hpp"


struct FViewer : public IViewer
{
public:
	FViewer(PlayerController* controller, SHARED(FEngineConfig) config);

public:
	class  World*           GetWorld();
	class  CameraComponent* GetActiveCamera();
	struct EventCollector*  GetEventCollector();
	class  CameraManager*   GetCameraManager();

protected:
	SHARED(FEngineConfig) config;
	PlayerController* controller;
};


#endif //!VIEWER_HPP