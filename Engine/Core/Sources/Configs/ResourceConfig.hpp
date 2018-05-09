#pragma once
#ifndef RESOURCE_CONFIG_HPP
#define RESOURCE_CONFIG_HPP


#include "Reflection/Archived.hpp"


struct FResourceConfig : public Archived
{
	GENERATED_ARCHIVATION_BODY(FResourcesConfig, Archived)
		ARCHIVE_MEMBER("", pathToResources)	
		ARCHIVATION_END()
public:
	std::string pathToResources = "Resources";
};



#endif // !ENGINE_CONFIG_HPP

