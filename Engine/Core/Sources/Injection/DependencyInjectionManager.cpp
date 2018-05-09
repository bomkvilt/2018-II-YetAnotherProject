#include "DependencyInjectionManager.hpp"
#include <string>
UNIQUE(IFrontendFabric) DependencyInjectionManager::frontend = nullptr;


void DependencyInjectionManager::SetFrontendFabric(UNIQUE(IFrontendFabric) fabric)
{
	frontend = std::move(fabric);
}

UNIQUE(IFacade) DependencyInjectionManager::MakeFacade(ActorComponent* owner, std::string relativePath)
{
	if (frontend)
	{
		return frontend->MakeFacade(owner, relativePath);
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
