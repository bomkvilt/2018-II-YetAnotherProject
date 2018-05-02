#include "PlayerController.hpp"

#include <assert.h>
#include <thread>

#include "World.hpp"
#include "Avatar.hpp"

#include "Injection/DependencyInjectionManager.hpp"


PlayerController::PlayerController()
	: cameraManager()
{
	if (auto init = ThreadContext::TopInitialiser())
	if (auto config = init->config)
	{
		viewer = DependencyInjectionManager::MakeViewer(this, config);
		
		return;
	}

	throw std::runtime_error("config must not be nulled");
}


PlayerController::~PlayerController()
{}

void PlayerController::OnBeginPlay()
{
	Super::OnBeginPlay();
	if (target)
	{
		EventBinder binder;

		target->SetupInput(&binder);

		eventManager.ProcessEventBinder(binder);
	}
}

void PlayerController::Tick(float DeltaTime, ETickType type)
{
	processInput();
}

void PlayerController::Render()
{
	if (viewer)
	{
		viewer->Render();
	}
}

void PlayerController::UnregisterCamera(CameraComponent* camera)
{
	cameraManager.UnregisterCamera(camera);
}

void PlayerController::RegisterCamera(CameraComponent* camera, bool bActivate)
{
	cameraManager.RegisterCamera(camera, bActivate);
}

void PlayerController::SetCameraActive(CameraComponent* camera, bool newState)
{
	cameraManager.SetCameraActive(camera, newState);
}

CameraComponent* PlayerController::GetActiveCamera()
{
	return cameraManager.GetCurrentCamera();
}

CameraManager& PlayerController::GetCameraManager()
{
	return cameraManager;
}

EventCollector& PlayerController::GetEventCollector()
{
	return eventCollector;
}

void PlayerController::processInput()
{
	auto& events  = eventCollector.events;
	float mouse_X = eventCollector.mouse_X;
	float mouse_Y = eventCollector.mouse_Y;

	eventManager.ProcessKeys(events);
	eventManager.UpdateMouseLocation(mouse_X, mouse_Y);

	eventCollector.Flush();
}
