#ifndef RESOURCESYSTEM_HPP
#define RESOURCESYSTEM_HPP

#include <string>
#include <unordered_map>
#include "Modules/IModule.hpp"
#include "Interfaces/IResource.hpp"
#include "Common.hpp"


class ResourceSystem : public IModule
{
public:
	ResourceSystem(SHARED(FEngineConfig) config) :
		IModule(config)
	{
	}

public:
	template<class T>
	SHARED(T) GetResource(std::string relativePath) 
	{
		if (resources.find(relativePath) == resources.end())
		{
			auto absolutePath = GetAubsolutePath(relativePath);
			auto newResource = std::make_shared<T>(absolutePath);
			resources[relativePath] = newResource;
			return newResource;
		}
		else return std::dynamic_pointer_cast<T>(resources[relativePath]);
	}

protected:
	std::string GetAubsolutePath(const std::string& relativePath)
	{
		if (config)
		{
			return config->resource.pathToResources + "/" + relativePath;
		}
		else throw std::runtime_error("no config was found");
	}

protected:
	std::unordered_map<std::string, SHARED(IResource)> resources;
};



#endif // !COMPONENT_VISUALISERS_HPP
