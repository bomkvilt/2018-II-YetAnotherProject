#include "ModuleManager.hpp"

ModuleManager ModuleManager::moduleManager = ModuleManager();

ModuleManager& ModuleManager::Get()
{
	return moduleManager;
}

ModuleManager::~ModuleManager()
{
	UnloadModules();
}

void ModuleManager::UnloadModules()
{	
	std::for_each(modules.rbegin(), modules.rend(), [&](auto& imodule)
	{
		imodule->OnUnload();
	});
	modules.clear();
}