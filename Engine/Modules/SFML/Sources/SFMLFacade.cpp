#include "SFMLFacade.hpp"
#include "ActorComponent.hpp"


Facade::Facade(ActorComponent* owner)
	: IFacade(owner)
{}

bool Facade::AttachTo(IFacade* parent)
{
	return true;
}

bool Facade::Detach()
{
	return true;
}

void Facade::Update()
{

	

	sf::Sprite sprite;
	sf::Texture texture;
	//texture.loadFromFile("box.jpg");
	sprite.setTexture(texture);

}

