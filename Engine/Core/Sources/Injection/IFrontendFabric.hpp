#ifndef IFRONTEND_FABRIC_HPP
#define IFRONTEND_FABRIC_HPP

#include "Interfaces/IFacade.hpp"
#include "Interfaces/IViewer.hpp"

class PlayerController;
class BaseActorComponent;



struct IFrontendFabric
{
	virtual UNIQUE(IFacade) MakeFacade(BaseActorComponent* owner) = 0;
	virtual UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config) = 0;
};



#endif // !IFRONTEND_FABRIC_HPP
