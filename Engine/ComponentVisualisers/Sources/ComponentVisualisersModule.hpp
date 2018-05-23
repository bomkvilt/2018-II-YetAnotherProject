#ifndef COMPONENT_VISUALISERS_HPP
#define COMPONENT_VISUALISERS_HPP
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Modules/IModule.hpp"
#include "Interfaces/IViewer.hpp"
#include "Misc.hpp"

#include "ComponentVisualiser.hpp"
class BaseActorComponent;


class ComponentVisualisersModule : public IModule
{
	using Visualisers = std::unordered_map<std::string, UNIQUE(ComponentVisualiser)>;

public:
	static ComponentVisualisersModule& Get();
	ComponentVisualisersModule::ComponentVisualisersModule(SHARED(FEngineConfig)& config)
		: IModule(config)
	{}
	virtual void OnLoaded() override;
	virtual void OnUnload() override;


public:
	// request a visualisation for the component
	void Visualise(BaseActorComponent* component, IViewer* viewer);

	// add visualiser for the component
	void AddVisualiser   (const std::string& className, UNIQUE(ComponentVisualiser) visualiser);
	void RemoveVisualiser(const std::string& classname);

private:
	Visualisers visualisers;
};



#endif // !COMPONENT_VISUALISERS_HPP
