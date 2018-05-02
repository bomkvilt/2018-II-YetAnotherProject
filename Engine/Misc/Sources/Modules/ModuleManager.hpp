#pragma once
#ifndef MODULE_MANAGER_HPP
#define MODULE_MANAGER_HPP

#include "IModule.hpp"
#include "Types.hpp"
#include "Misc.hpp"

#include <memory>
#include <deque>
#include <algorithm>


class ModuleManager
{
public:
	static ModuleManager& Get();

	~ModuleManager();

	// unload all modules
	void UnloadModules();

public:
	template<class _M> 
		_M& GetModule()
		{	// Get loaded module or create a new one
		for (auto& imodule : modules)
			{
			if (auto* module = dynamic_cast<_M*>(imodule.get()))
				{
				return *module;
				}
			}
		return CreateModule<_M>();
		}

	template<class _M>
		_M& CreateModule()
		{	// create a module
		auto ptr = std::make_unique<_M>();
		auto raw = ptr.get();
		modules.emplace_back(std::move(ptr));
		raw->OnLoaded();
		return *raw;
		}

	template<class _M>
		void LoadModule()
		{
		CreateModule<_M>();
		}

	template<class _M>
		void UnloadMolule()
		{	// unload the module 
		for (auto& imodule : modules)
			{
			if (auto* module = dynamic_cast<_M*>(imodule.get()))
				{
				module->OnUnload();
				modules.erase(imodule);
				return;
				}
			}
		}

private:
	ModuleManager() = default;

	std::deque<UNIQUE(IModule)> modules;

	static ModuleManager moduleManager;
};


#endif // !MODULE_MANAGER_HPP
