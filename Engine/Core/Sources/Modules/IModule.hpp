#pragma once
#ifndef IMODULE_HPP
#define IMODULE_HPP

#include "Configs/EngineConfig.hpp"


class IModule
{
public:
	IModule(SHARED(FEngineConfig)& config)
		: config(config)
	{}
	
	virtual void OnLoaded() {}
	virtual void OnUnload() {}

public:
	SHARED(FEngineConfig) config;
};



#endif //!IMODULE_HPP
