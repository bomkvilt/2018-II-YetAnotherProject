#pragma once

#include "GameMode.hpp"

class PDGameMode : public GameMode
{
public:
	using Super = GameMode;

public:

	PDGameMode();

public:
	/// >> SimulationMode
	virtual void OnBeginPlay() override;
	virtual void OnEndPlay()   override;

	virtual void Tick(float DeltaTime, ETickType type) override;
	/// <<
};