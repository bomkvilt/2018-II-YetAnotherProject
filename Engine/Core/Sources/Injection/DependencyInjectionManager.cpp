#include "DependencyInjectionManager.hpp"

UNIQUE(IFrontendFabric) DependencyInjectionManager::frontend = nullptr;
UNIQUE(IPhysicsFabric ) DependencyInjectionManager::physics  = nullptr;


void DependencyInjectionManager::SetFrontendFabric(UNIQUE(IFrontendFabric) fabric)
{
	frontend = std::move(fabric);
}

UNIQUE(IFacade) DependencyInjectionManager::MakeFacade(BaseActorComponent* owner)
{
	if (frontend)
	{
		return frontend->MakeFacade(owner);
	}
	return nullptr;
}

UNIQUE(IViewer) DependencyInjectionManager::MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config)
{
	if (frontend)
	{
		return frontend->MakeViewer(controller, config);
	}
	return nullptr;
}

void DependencyInjectionManager::SetPhysicsConfig(UNIQUE(IPhysicsFabric) conf)
{
	physics = std::move(conf);
}

UNIQUE(IPhysicsScene) DependencyInjectionManager::MakeScene()
{
	if (physics)
	{
		return physics->MakePhysicsScene();
	}
	return nullptr;
}

UNIQUE(IConstraint) DependencyInjectionManager::MakeConstraint(BaseActorComponent* owner)
{
	if (physics)
	{
		return physics->MakeConstraint(owner);
	}
	return nullptr;
}

UNIQUE(IRigidBody) DependencyInjectionManager::MakeBody(FShape shape, BaseActorComponent* owner, float mass, FVector inertia)
{
	if (physics)
	{
		return physics->MakeRigidBody(shape, owner, mass, inertia);
	}
	return nullptr;
}
