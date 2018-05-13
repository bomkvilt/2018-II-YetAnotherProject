#pragma once
#ifndef OSG_FABRIC_HPP
#define OSG_FABRIC_HPP

#include "Injection/IFrontendFabric.hpp"
#include "OSGViewer.hpp"



struct OSGFabric : public IFrontendFabric
{
	static UNIQUE(IFrontendFabric) Get()
	{
		return std::make_unique<OSGFabric>();
	}

	virtual UNIQUE(IFacade) MakeFacade(BaseActorComponent* owner) 
	{ 
		return std::make_unique<Facade>(owner); 
	}

	virtual UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config)
	{ 
		return std::make_unique<Viewer>(controller, config); 
	}
};


#endif // !OSG_FABRIC_HPP
