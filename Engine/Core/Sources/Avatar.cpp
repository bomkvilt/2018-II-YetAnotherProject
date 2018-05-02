#include "Avatar.hpp"
#include "PlayerController.hpp"

Avatar::Avatar()
{
	auto* init = ThreadContext::TopInitialiser();
	assert(init);
	controller = dynamic_cast<PlayerController*>(init->controller);
}
