#include "Avatar.hpp"
#include "PlayerController.hpp"

Avatar::Avatar()
{
	if (auto* init = ThreadContext::TopInitialiser())
	{
		controller = dynamic_cast<PlayerController*>(init->controller);
	}
	movement = CreateSubModule<AvatarMovementComponent>("Movement");
}
