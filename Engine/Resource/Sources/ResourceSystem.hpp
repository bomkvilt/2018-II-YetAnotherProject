#ifndef RESOURCESYSTEM_HPP
#define RESOURCESYSTEM_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include "Modules/IModule.hpp"
#include "Misc.hpp"
#include "Modules/IResource.hpp"

class ResourceSystem : public IModule
{
	ResourceSystem(SHARED(FEngineConfig) config) :
		IModule(config)
	{
	};

public:
	template<class T>
	SHARED(T) GetResource(std::string relativePath) {
		if (resources.find(relativePath) == resources.end())
		{
			
			SHARED(T) newResource = new std::make_shared<T>(relativePath);;
			resources.insert(std::make_pair(relativePath, newResource));
			}
	};

protected:
	std::unordered_map<std::string, SHARED(IResource)> resources;
	

	

};



#endif // !COMPONENT_VISUALISERS_HPP
