#ifndef SFML_MODULE_CONFIG_HPP
#define SFML_MODULE_CONFIG_HPP

#include "Configs/FrontendConfig.hpp"

#include "SFMLViewer.hpp"


struct SFMLConfig : public FFrontendConfig
{
	virtual IFacade* MakeFacade(ActorComponent* owner) 
	{ 
		return new Facade(owner); 
	}

	virtual IViewer* MakeViewer(PlayerController* controller) 
	{ 
		return new Viewer(controller); 
	}
};


#endif // !SFML_MODULE_CONFIG_HPP
