#ifndef MODULE_MANAGER_HPP
#define MODULE_MANAGER_HPP

#include "IFrontendFabric.hpp"


class DependencyInjectionManager
{
public:
	static void SetFrontendFabric(UNIQUE(IFrontendFabric) fabric);
	static UNIQUE(IFacade) MakeFacade(ActorComponent* owner);
	static UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config);

private:

	static UNIQUE(IFrontendFabric) frontend;
};


#endif // !MODULE_MANAGER_HPP
