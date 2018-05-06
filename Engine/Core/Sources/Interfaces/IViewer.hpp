#ifndef IVIEWER_HPP
#define IVIEWER_HPP

#include <memory>
#include "Types.hpp"
#include "Misc.hpp"
#include "Configs/EngineConfig.hpp"

class World;
class CameraComponent;
class CameraManager;
class PlayerController;
struct EventCollector;



struct IViewer
{
	IViewer(PlayerController* controller, SHARED(FEngineConfig) config)
		: controller(controller)
		, config    (config)
	{}
	virtual ~IViewer() = default;

public:
	virtual void Render() = 0;
	virtual void DrawShape(FShape shape, FTransform transform, FColor color) = 0;

public:
	World*			 GetWorld();
	CameraComponent* GetActiveCamera();
	EventCollector*  GetEventCollector();
	CameraManager*   GetCameraManager();

protected:
	SHARED(FEngineConfig) config;
	PlayerController* controller;
};


#endif // !IVIEWER_HPP
