#include "CameraManager.hpp"
#include "../World.hpp"

#include <assert.h>

#define NOT_SELECTED -1


CameraManager::CameraManager()
	:activeCamera(NOT_SELECTED)
{}

void CameraManager::UnregisterCamera(CameraComponent* camera)
{
	assert(camera);

	int pos = FindCamera(camera);
	if (pos != NOT_SELECTED)
	{
		ActivateNext(pos);
		RemoveCamera(pos);
	}
}

void CameraManager::RegisterCamera(CameraComponent* camera, bool bActivate)
{
	assert(camera);

	int pos = PlaceCamera(camera);
	assert(pos != NOT_SELECTED);

	if (bActivate)
	{
		SetCameraActive(camera, bActivate);
	}
}

void CameraManager::SetCameraActive(CameraComponent* camera, bool newState)
{
	assert(camera);

	if (newState)
	{
		int pos = FindCamera(camera);
		assert(pos != NOT_SELECTED);
		ActivateNext(--pos);
	}
	else
	{
		int pos = FindCamera(camera);
		assert(pos != NOT_SELECTED);
		ActivateNext(pos);
	}
}

CameraComponent* CameraManager::GetCurrentCamera()
{
	if (activeCamera != NOT_SELECTED)
	{
		return cameras[activeCamera];
	}
	return nullptr;
}

const CameraComponent* CameraManager::GetCurrentCamera() const
{
	if (activeCamera != NOT_SELECTED)
	{
		return cameras[activeCamera];
	}
	return nullptr;
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
	return NOT_SELECTED;
}

int CameraManager::PlaceCamera(CameraComponent* camera)
{
	cameras.emplace_back(camera);
	return (int)cameras.size() - 1;
}

void CameraManager::ActivateNext(int currentPos)
{
	int size = (int)cameras.size();

	// if we have another camera to switch on
	// or we add a first one
	if (size > 2 || currentPos == NOT_SELECTED)
	{
		activeCamera = (currentPos + 1) % size;
	}
	else
	{
		activeCamera = NOT_SELECTED;
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
