#ifndef IRESOURCE_HPP
#define IRESOURCE_HPP
#include "Types.hpp"
#include <string>
#include "Misc.hpp"
#include <memory>


#include "Configs/EngineConfig.hpp"
class IResource : std::enable_shared_from_this<IResource>
{
public:
	IResource(std::string relativePath) :
		resourcesPath(relativePath)
	{
	};


	 ~IResource();




protected:
	std::string resourcesPath;

};

#endif // !IModule_HPP

