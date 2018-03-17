#pragma once

#include "Core/SymulationMode.hpp"

class PendulumSymulationMode : public SymulationMode
{
public:

	using Super = SymulationMode;

public:

	PendulumSymulationMode();

	static UNIQUE(SymulationMode) Get();

private:

	static PlayerController* MakeController();

public:
	/// >> SymulationMode
	virtual void OnSymulationBegin() override;
	virtual void OnSymulationEnd()	 override;
	virtual void StopSymulation()	 override;

	virtual void DoTick(float deltaTime, ETickType type) override;
	/// <<
};