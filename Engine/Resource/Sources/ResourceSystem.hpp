#ifndef RESOURCESYSTEM_HPP
#define RESOURCESYSTEM_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Modules/IModule.hpp"
#include "Interfaces/IViewer.hpp"
#include "Misc.hpp"
#include <string>
#include "ComponentVisualiser.hpp"

#include "Modules\IResource.hpp"

class ResourseSystem : public IModule
{
	

public:
	

	
	SHARED(IResource) GetResource( std::string relativePath);

protected:
	std::unordered_map<std::string, SHARED(IResource)> resources;
	

	

};



#endif // !COMPONENT_VISUALISERS_HPP
