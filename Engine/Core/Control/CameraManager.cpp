#include "CameraManager.hpp"

#include <assert.h>

#include "../World.hpp"
#include "../../Components/CameraComponent.hpp"


CameraManager::CameraManager(osgViewer::Viewer& viewer, World* world)
	: viewer(viewer)
	, world	(world)
	, activeCamera(-1)
{}

void CameraManager::UnregisterCamera(CameraComponent* camera)
{
	assert(IsValid(camera));

	int pos = FindCamera(camera);
	if (pos != -1)
	{
		ActivateNext(pos);
		RemoveCamera(pos);
	}
}

void CameraManager::RegisterCamera(CameraComponent* camera, bool bActivate)
{
	assert(IsValid(camera));

	int pos = PlaceCamera(camera);
	assert(pos != -1);

	if (bActivate)
	{
		SetCameraActive(camera, bActivate);
	}
}

void CameraManager::SetCameraActive(CameraComponent* camera, bool newState)
{
	assert(IsValid(camera));

	if (newState)
	{
		int pos = FindCamera(camera);
		assert(pos != -1);
		ActivateNext(--pos);
	}
	else
	{
		int pos = FindCamera(camera);
		assert(pos != -1);
		ActivateNext(pos);
	}
}

CameraComponent* CameraManager::GetCurrentCamera()
{
	if (activeCamera != -1)
	{
		return cameras[activeCamera];
	}
	return nullptr;
}

const CameraComponent* CameraManager::GetCurrentCamera() const
{
	if (activeCamera != -1)
	{
		return cameras[activeCamera];
	}
	return nullptr;
}

bool CameraManager::IsValid(CameraComponent* camera)
{
	return world && world->IsValid(camera);
}

int CameraManager::FindCamera(CameraComponent* camera)
{
	for (int i = 0; i < cameras.size(); ++i)
	{
		if (cameras[i] == camera)
		{
			return i;
		}
	}
	return -1;
}

int CameraManager::PlaceCamera(CameraComponent* camera)
{
	cameras.emplace_back(camera);
	return cameras.size() - 1;
}

void CameraManager::ActivateNext(int currentPos)
{
	int size = cameras.size();

	if (size > 2 || currentPos == -1)
	{
		activeCamera = (currentPos + 1) % size;
	}
	else
	{
		activeCamera = -1;
	}
}

void CameraManager::RemoveCamera(int currentPos)
{
	auto begin = cameras.begin();
	cameras.erase(begin + currentPos);
	
	if (activeCamera > currentPos)
	{
		--activeCamera;
	}
}
