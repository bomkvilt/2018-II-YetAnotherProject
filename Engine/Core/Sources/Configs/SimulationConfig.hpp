#pragma once
#ifndef SIMULATION_CONFIG_HPP
#define SIMULATION_CONFIG_HPP

#include "Reflection/Archived.hpp"



struct FSimulationConfig : public Archived
{
	GENERATED_ARCHIVATION_BODY(FSimulationConfig, Archived)
		ARCHIVE_MEMBER("", stepTime)
		ARCHIVATION_END()
public:
	float stepTime = 1/30.f;
};



#endif // !SIMULATION_CONFIG_HPP
