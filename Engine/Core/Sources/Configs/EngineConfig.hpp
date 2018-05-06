#pragma once
#ifndef ENGINE_CONFIG_HPP
#define ENGINE_CONFIG_HPP

#include "FrontendConfig.hpp"
#include "SimulationConfig.hpp"



struct FEngineConfig : public Archived
{
	GENERATED_ARCHIVATION_BODY(FEngineConfig, Archived)
		ARCHIVE_MEMBER("", viewer    )
		ARCHIVE_MEMBER("", simulation)
		ARCHIVATION_END()
public:
	FViewerConfig     viewer;
	FSimulationConfig simulation;
};



#endif // !ENGINE_CONFIG_HPP

