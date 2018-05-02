#include "Viewer.hpp"
#include "PlayerController.hpp"


FViewer::FViewer(PlayerController* controller, SHARED(FEngineConfig) config)
	: controller(controller)
	, config    (config)
{}

World* FViewer::GetWorld()
{
	if (controller)
	{
		return controller->GetWorld();
	}
	return nullptr;
}

CameraComponent* FViewer::GetActiveCamera()
{
	if (controller)
	{
		return controller->GetActiveCamera();
	}
	return nullptr;
}

EventCollector* FViewer::GetEventCollector()
{
	if (controller)
	{
		return &controller->GetEventCollector();
	}
	return nullptr;
}

CameraManager* FViewer::GetCameraManager()
{
	if (controller)
	{
		return &controller->GetCameraManager();
	}
	return nullptr;
}