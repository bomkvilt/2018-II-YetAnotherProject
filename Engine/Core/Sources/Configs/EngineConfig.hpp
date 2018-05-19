#pragma once
#ifndef ENGINE_CONFIG_HPP
#define ENGINE_CONFIG_HPP

#include "FrontendConfig.hpp"
#include "SimulationConfig.hpp"



struct FEngineConfig : public Archived
{
	GENERATED_CONFIG_BODY(FEngineConfig, Archived)
		ARCHIVE_MEMBER("", frontend      )
		ARCHIVE_MEMBER("", simulation    )
		ARCHIVE_MEMBER("", countOfThreads)
		ARCHIVATION_END()
public:
	FFrontendConfig   frontend;
	FSimulationConfig simulation;

	size_t countOfThreads = 1;
};



#endif // !ENGINE_CONFIG_HPP

