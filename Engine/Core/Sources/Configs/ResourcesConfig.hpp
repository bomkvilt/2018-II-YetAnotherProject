
#ifndef RESOURCESCONFIG_CONFIG_HPP
#define RESOURCESCONFIG_CONFIG_HPP

#include "FrontendConfig.hpp"
#include "SimulationConfig.hpp"
#include "Reflection/Archived.hpp"


struct FResourcesConfig : public Archived
{
	GENERATED_ARCHIVATION_BODY(FViewerConfig, Archived)
		ARCHIVE_MEMBER("", pathToResources)
	
		ARCHIVATION_END()
public:
	std::string pathToResources = "Default Name";
	
};



#endif // !ENGINE_CONFIG_HPP

