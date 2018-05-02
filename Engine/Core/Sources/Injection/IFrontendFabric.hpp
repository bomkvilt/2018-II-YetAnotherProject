#ifndef IFRONTEND_FABRIC_HPP
#define IFRONTEND_FABRIC_HPP

#include "Interfaces/IFacade.hpp"
#include "Interfaces/IViewer.hpp"

class PlayerController;
class ActorComponent;



struct IFrontendFabric
{
	virtual UNIQUE(IFacade) MakeFacade(ActorComponent* owner) = 0;
	virtual UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config) = 0;
};



#endif // !IFRONTEND_FABRIC_HPP
