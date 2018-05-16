#pragma once
#ifndef ENGINE_CONFIG_HPP
#define ENGINE_CONFIG_HPP

#include "FrontendConfig.hpp"
#include "SimulationConfig.hpp"
#include "ResourceConfig.hpp"


struct FEngineConfig : public Archived
{
	GENERATED_ARCHIVATION_BODY(FEngineConfig, Archived)
		ARCHIVE_MEMBER("", viewer    )
		ARCHIVE_MEMBER("", simulation)
		ARCHIVE_MEMBER("", resource  )
		ARCHIVE_MEMBER("", countOfThreads)
		ARCHIVATION_END()
public:
	FViewerConfig     viewer;
	FSimulationConfig simulation;
	FResourceConfig   resource;

	size_t countOfThreads = 1;
};



#endif // !ENGINE_CONFIG_HPP

