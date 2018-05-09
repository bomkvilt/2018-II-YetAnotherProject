#ifndef IRESOURCE_HPP
#define IRESOURCE_HPP
#include "Types.hpp"
#include<string>
#include "Misc.hpp"
#include <memory>


#include "Configs/EngineConfig.hpp"
class IResource
{
public:

	IResource(SHARED(FEngineConfig) config) :
		config(config)
	{
		OnLoaded();
			
	};


	 ~IResource();

protected:

	virtual void OnLoaded() = 0;
	virtual void OnUnloaded() = 0;



protected:
	std::string resourcesPath;
	SHARED(FEngineConfig) config;
};

#endif // !IModule_HPP

