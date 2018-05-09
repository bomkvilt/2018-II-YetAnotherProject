
#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "ActorComponent.hpp"
#include "Injection\DependencyInjectionManager.hpp"
class SpriteComponent : public ActorComponent
{
public:

	SpriteComponent(std::string pathToSprite) {
		
		facade = DependencyInjectionManager::MakeFacade(this, pathToSprite);
		
	}

protected:


};





#endif // !CAMERA_HPP
