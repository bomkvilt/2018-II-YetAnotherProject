#ifndef PENDULUM_PLAYER_CONTROLLER_HPP
#define PENDULUM_PLAYER_CONTROLLER_HPP

#include "Core/PlayerController.hpp"



class PendulumPlayerController : public PlayerController
{
public:
	PendulumPlayerController()
	{
		eventManager.AddAxis("R1", 
			{
				{KEY_W,  1},
				{KEY_S, -1}
			});
		eventManager.AddAxis("R2", 
			{
				{KEY_E,  1},
				{KEY_D, -1}
			});
		eventManager.AddAxis("R3", 
			{
				{KEY_R,  1},
				{KEY_F, -1}
			});
		eventManager.AddAxis("Move", 
			{
				{KEY_B,  1},
				{KEY_N, -1}
			});
		eventManager.AddAction("Push",	  KEY_X);
		eventManager.AddAction("Click_L", KEY_MOUSE_L);
		eventManager.AddAction("Click_R", KEY_MOUSE_R);
	}
};






#endif // !PENDULUM_PLAYER_CONTROLLER_HPP
