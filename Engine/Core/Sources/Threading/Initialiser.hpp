#ifndef INITIALISER_HPP
#define INITIALISER_HPP

#include <string>
#include <memory>
#include "Misc.hpp"
#include "Types.hpp"


class World;
class Object;
class Actor;
class SimulationMode;
class BasePlayerController;
struct FEngineConfig;

struct Initialiser
{
	OUID        ouid;
	EObjectType type = EObjectType::eObject;
	SHARED(FEngineConfig) config;
	
	World*                world      = nullptr;
	Actor*                owner      = nullptr;
	std::string*          name       = nullptr;
	SimulationMode*       simulation = nullptr;
	BasePlayerController* controller = nullptr;

public:
	static UNIQUE(Initialiser) Get()
	{ 
		return std::make_unique<Initialiser>();
	}

	 Initialiser() = default;
	~Initialiser() = default;
};

#endif // !INITIALISER_HPPs