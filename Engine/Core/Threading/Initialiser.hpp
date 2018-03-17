#ifndef INITIALISER_HPP
#define INITIALISER_HPP

#include <string>
#include "../Types.hpp"


class World;
class Object;
class Actor;
class PlayerController;

struct Initialiser
{
	OUID		 ouid;
	World*		 world	= nullptr;
	std::string* name	= nullptr;
	Actor*		 owner	= nullptr;
	PlayerController* controller = nullptr;
};

#endif // !INITIALISER_HPPs