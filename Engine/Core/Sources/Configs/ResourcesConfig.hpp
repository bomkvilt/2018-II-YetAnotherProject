
#ifndef RESOURCES_CONFIG_HPP
#define RESOURCES_CONFIG_HPP


#include "Reflection/Archived.hpp"


struct FResourcesConfig : public Archived
{
	GENERATED_ARCHIVATION_BODY(FResourcesConfig, Archived)
		ARCHIVE_MEMBER("", pathToResources)
	
		ARCHIVATION_END()
public:
	std::string pathToResources = "Default Name";
	
};



#endif // !ENGINE_CONFIG_HPP

