#include "Engine.hpp"
#include "SFMLFabric.hpp"
#include "Injection/DependencyInjectionManager.hpp"

#include "PDGameMode.hpp"
#include "PDPlayerController.hpp"
#include "Pendulum.hpp"


using PDSimulationFabric = TSimulationModeFabric<SimulationMode, PDPlayerController, PDGameMode>;



int main()
{
	DependencyInjectionManager::SetFrontendFabric(SFMLFabric::Get());

	auto simulationFabric = PDSimulationFabric::Get();
	
	Engine engine;
	engine.SetPathToConfig("EngineConfig.txt");
	engine.SetSimulationFabric(std::move(simulationFabric));
	engine.SaveConfig();
	return engine.MainCycle();
}