#include "IViewer.hpp"
#include "IViewer.hpp"
#include "../PlayerController.hpp"


World* IViewer::GetWorld()
{
	if (controller)
	{
		return controller->GetWorld();
	}
	return nullptr;
}

CameraComponent* IViewer::GetActiveCamera()
{
	if (controller)
	{
		return controller->GetActiveCamera();
	}
	return nullptr;
}

EventCollector* IViewer::GetEventCollector()
{
	if (controller)
	{
		return &controller->GetEventCollector();
	}
	return nullptr;
}

CameraManager* IViewer::GetCameraManager()
{
	if (controller)
	{
		return &controller->GetCameraManager();
	}
	return nullptr;
}

