#ifndef BASE_PLAYER_CONTROLLER_HPP
#define BASE_PLAYER_CONTROLLER_HPP
#pragma once

#include "Common.hpp"

class World;
class Avatar;
class GameMode;


class BasePlayerController : public Archived
{
	GENERATED_BODY(BasePlayerController, Archived);
public:

	BasePlayerController();
	virtual ~BasePlayerController() = default;

public:

	virtual void OnBeginPlay();
	virtual void OnEndPlay();

	virtual void Tick(float DeltaTime, ETickType type);

public:

	const World* GetWorld() const		{ return world;			}
	      World* GetWorld()				{ return world;			}
	void SetWorld(World* newWorld)		{ world = newWorld;		}

	const Avatar* GetTarget() const		{ return target;		}
	      Avatar* GetTarget()			{ return target;		}
	void SetTarget(Avatar* newTarget)	{ target = newTarget;	}

	const GameMode* GetGameMode() const { return gameMode;		}
	      GameMode* GetGameMode()       { return gameMode;		}
	void SetGameMode(GameMode* newMode) { gameMode = newMode;   }

protected:
	/// << 
	Avatar*   target;
	World*    world;
	GameMode* gameMode;
	/// >>
};



#endif // !BASE_PLAYER_CONTROLLER_HPP
