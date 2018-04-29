#include "SFMLFacade.hpp"
#include "ActorComponent.hpp"


Facade::Facade(ActorComponent* owner)
	: IFacade(owner, FShape::MakeBox({1,1,0}))
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
	assert(shape.type == EShapeType::eBox);

	//** create radius-vectors to the shape projection's vertices (Oxy)
	FVector extends = shape.extends;
	FTransform transform= owner->GetComponentTransform();

	

	sf::Sprite sprite;
	sf::Texture texture;
	//texture.loadFromFile("box.jpg");
	sprite.setTexture(texture);

	sprite.setTextureRect(sf::IntRect(transform.Location.X, transform.Location.Y, extends.X * 2, extends.Y * 2));
}

