#include "Avatar.hpp"



Avatar::Avatar()
	: controller(ThreadContextSingleton::Get().TopInitialiser()->controller)
{}
