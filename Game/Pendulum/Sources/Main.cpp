#include "Engine.hpp"
#include "B2Fabric.hpp"
#include "SFMLFabric.hpp"
#include "Injection/DependencyInjectionManager.hpp"

#include "PDGameMode.hpp"
#include "PDPlayerController.hpp"
#include "Pendulum.hpp"


using PDSimulationFabric = TSimulationModeFabric<SimulationMode, PDPlayerController, PDGameMode>;



int main()
{
	DependencyInjectionManager::SetFrontendFabric(SFMLFabric::Get());
	DependencyInjectionManager::SetPhysicsConfig (B2Fabric ::Get());

	auto simulationFabric = PDSimulationFabric::Get();
	
	Engine engine;
	engine.SetPathToConfig("EngineConfig.txt");
	engine.SetSimulationFabric(std::move(simulationFabric));
	engine.SaveConfig();
	return engine.MainCycle();
}