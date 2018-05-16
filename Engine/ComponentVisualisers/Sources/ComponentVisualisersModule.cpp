#include "ComponentVisualisersModule.hpp"
#include "BaseActorComponent.hpp"
#include "ComponentVisualiser.hpp"

#include "Modules/ModuleManager.hpp"
#include "Visualisers/BoxColisionVisualiser.hpp"


ComponentVisualisersModule& ComponentVisualisersModule::Get()
{
	auto& manager = ModuleManager::Get();
	return manager.GetModule<ComponentVisualisersModule>();
}

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
