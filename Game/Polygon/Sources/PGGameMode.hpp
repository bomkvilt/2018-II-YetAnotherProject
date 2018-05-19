#pragma once

#include "GameMode.hpp"

class PGGameMode : public GameMode
{
public:
	using Super = GameMode;

public:

	PGGameMode();

public:
	/// >> SimulationMode
	virtual void Tick(float DeltaTime, ETickType type) override;
	/// <<
};