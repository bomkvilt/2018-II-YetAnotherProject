#ifndef SFML_MODULE_CONFIG_HPP
#define SFML_MODULE_CONFIG_HPP

#include "Injection/IFrontendFabric.hpp"

#include "SFMLViewer.hpp"


struct SFMLFabric : public IFrontendFabric
{
	static UNIQUE(IFrontendFabric) Get()
	{
		return std::make_unique<SFMLFabric>();
	}

	virtual UNIQUE(IFacade) MakeFacade(ActorComponent* owner) override
	{
		return std::make_unique<Facade>(owner);
	}

	virtual UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config) override
	{
		return std::make_unique<Viewer>(controller, config);
	}
};



#endif // !SFML_MODULE_CONFIG_HPP
