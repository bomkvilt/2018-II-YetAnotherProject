#ifndef PENDULUM_PLAYER_CONTROLLER_HPP
#define PENDULUM_PLAYER_CONTROLLER_HPP
#pragma once

#include "PlayerController.hpp"


class PGPlayerController : public PlayerController
{
public:
	PGPlayerController()
	{
		eventManager.AddAxis("Fwd", 
			{
				{KEY_D,  1},
				{KEY_A, -1}
			});
		eventManager.AddAxis("R", 
			{
				{KEY_R,  1},
				{KEY_F, -1}
			});
		eventManager.AddAction("Jump", ControlKey(KEY_X));
	}
};






#endif // !PENDULUM_PLAYER_CONTROLLER_HPP
