#include "Engine.hpp"
#include "B2Fabric.hpp"
#include "SFMLFabric.hpp"
#include "Injection/DependencyInjectionManager.hpp"

#include "TGGameMode.hpp"
#include "TGPlayerController.hpp"


using TGSimulationConfig = TSimulationModeFabric<SimulationMode, TGPlayerController, TGGameMode>;


int main()
{
	DependencyInjectionManager::SetFrontendFabric(SFMLFabric::Get());
	DependencyInjectionManager::SetPhysicsConfig (B2Fabric  ::Get());

	auto simulationFabric = TGSimulationConfig::Get();
	
	Engine engine;
	engine.SetPathToConfig("EngineConfig.txt");
	engine.SetSimulationFabric(std::move(simulationFabric));
	engine.SaveConfig();
	return engine.MainCycle();
}