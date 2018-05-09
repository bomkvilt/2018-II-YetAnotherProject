#include "ComponentVisualisersModule.hpp"
#include "ActorComponent.hpp"
#include "ComponentVisualiser.hpp"

#include "Modules/ModuleManager.hpp"
#include "Visualisers/BoxColisionVisualiser.hpp"


ComponentVisualisersModule& ComponentVisualisersModule::Get()
{
	return ModuleManager::Get().GetModule<ComponentVisualisersModule>();
}

ComponentVisualisersModule::ComponentVisualisersModule(SHARED(FEngineConfig)& config)
	: IModule(config)
{}

void ComponentVisualisersModule::OnLoaded()
{
	visualisers["BoxColision"] = std::make_unique<BoxColisionVisualiser>();
}

void ComponentVisualisersModule::OnUnload()
{
}

void ComponentVisualisersModule::Visualise(ActorComponent* component, IViewer* viewer)
{
	if (!component || !viewer) return;

	std::string className = component->__GetClassName();
	auto itr = visualisers.find(className);
	auto end = visualisers.end();
	if (itr != end)
	{
		auto helper = ComponentVisualiserHelper(viewer);
		if (auto visualiser = itr->second.get())
		{
			visualiser->Visualize(component, helper);
		}
	}
}

void ComponentVisualisersModule::AddVisualiser(const std::string& className, UNIQUE(ComponentVisualiser) visualiser)
{
	visualisers[className] = std::move(visualiser);
}

void ComponentVisualisersModule::RemoveVisualiser(const std::string & classname)
{
	if (visualisers.count(classname))
	{
		visualisers.erase(classname);
	}
}
