#ifndef IRESOURCE_HPP
#define IRESOURCE_HPP

#include <string>
#include "Common.hpp"
#include "Configs/EngineConfig.hpp"


class IResource : std::enable_shared_from_this<IResource>
{
public:
	IResource(std::string relativePath) :
		resourcesPath(relativePath)
	{
	}

	virtual ~IResource() = default;

protected:
	std::string resourcesPath;
};

#endif // !IModule_HPP

