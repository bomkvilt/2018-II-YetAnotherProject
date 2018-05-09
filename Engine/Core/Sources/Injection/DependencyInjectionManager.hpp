#ifndef MODULE_MANAGER_HPP
#define MODULE_MANAGER_HPP

#include "IFrontendFabric.hpp"
#include <string>

class DependencyInjectionManager
{
public:
	static void SetFrontendFabric(UNIQUE(IFrontendFabric) fabric);
	static UNIQUE(IFacade) MakeFacade(ActorComponent* owner, std::string relativePath);
	static UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config);

private:

	static UNIQUE(IFrontendFabric) frontend;
};


#endif // !MODULE_MANAGER_HPP
