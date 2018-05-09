#ifndef IFRONTEND_FABRIC_HPP
#define IFRONTEND_FABRIC_HPP

#include "Interfaces/IFacade.hpp"
#include "Interfaces/IViewer.hpp"
#include <string>
class PlayerController;
class ActorComponent;



struct IFrontendFabric
{
	virtual UNIQUE(IFacade) MakeFacade(ActorComponent* owner, std::string relativePath) = 0;
	virtual UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config) = 0;
};



#endif // !IFRONTEND_FABRIC_HPP
