#ifndef PENDULUM_PLAYER_CONTROLLER_HPP
#define PENDULUM_PLAYER_CONTROLLER_HPP
#pragma once

#include "PlayerController.hpp"


class PDPlayerController : public PlayerController
{
public:
	PDPlayerController()
	{
		eventManager.AddAxis("Fwd", 
			{
				{KEY_W,  1},
				{KEY_S, -1}
			});
		eventManager.AddAxis("Bwd", 
			{
				{KEY_A,  1},
				{KEY_D, -1}
			});
		eventManager.AddAxis("R1", 
			{
				{KEY_R,  1},
				{KEY_F, -1}
			});
		eventManager.AddAction("Click_L", KEY_MOUSE_L);
		eventManager.AddAction("Click_R", KEY_MOUSE_R);
	}
};






#endif // !PENDULUM_PLAYER_CONTROLLER_HPP
