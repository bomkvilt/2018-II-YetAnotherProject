#ifndef IFRONTEND_FABRIC_HPP
#define IFRONTEND_FABRIC_HPP

#include "Interfaces/IFacade.hpp"
#include "Interfaces/IViewer.hpp"
#include <string>
class PlayerController;
class BaseActorComponent;



struct IFrontendFabric
{
	virtual UNIQUE(IFacade) MakeFacade(BaseActorComponent* owner, std::string relativePath) = 0;
	virtual UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config) = 0;
};



#endif // !IFRONTEND_FABRIC_HPP
