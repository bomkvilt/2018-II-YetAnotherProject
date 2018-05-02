#include "DependencyInjectionManager.hpp"

UNIQUE(IFrontendFabric) DependencyInjectionManager::frontend = nullptr;



void DependencyInjectionManager::SetFrontendFabric(UNIQUE(IFrontendFabric) fabric)
{
	frontend = std::move(fabric);
}

UNIQUE(IFacade) DependencyInjectionManager::MakeFacade(ActorComponent* owner)
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
