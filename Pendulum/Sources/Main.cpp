#include "Engine.hpp"
#include "OSGConfig.hpp"
#include "Injection/DependencyInjectionManager.hpp"

#include "PDGameMode.hpp"
#include "PDPlayerController.hpp"
#include "Pendulum.hpp"


using PDSimulationFabric = TSimulationModeFabric<SimulationMode, PDPlayerController, PDGameMode>;



int main()
{
	DependencyInjectionManager::SetFrontendFabric(OSGFabric::Get());

	auto simulationFabric = PDSimulationFabric::Get();
	
	Engine engine;
	engine.SetPathToConfig("EngineConfig.txt");
	engine.SetSimulationFabric(std::move(simulationFabric));
	engine.SaveConfig();
	return engine.MainCycle();
}