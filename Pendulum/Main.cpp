#include <memory>

#include "Core/Engine.hpp"

#include "PendulumSymulationMode.hpp"
#include "Pendulum.hpp"

int main()
{
	Engine engine;
	auto mode = PendulumSymulationMode::Get();
	engine.SetSymulationMode(std::move(mode));
	return engine.MainCycle();
}