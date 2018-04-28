#include "SFMLViewer.hpp"

#include "World.hpp"
#include "ActorComponent.hpp"
#include "ComponentVisualisers.hpp"

#include <iostream>


Viewer::Viewer(PlayerController* controller)
	: IViewer(controller)
{
	window.create(sf::VideoMode(800, 600), "test");
}

void Viewer::Render()
{
	window.clear();

	sf::Event event;
	while(window.pollEvent(event)) // the sheet doesn't works without the stuff
	{
		if(event.type == sf::Event::Closed)
			window.close();
	}

	World& world = *GetWorld();
	for (auto& component : world)
	{
		ComponentVisualisers::Visualise(&component, this);
		auto* tmp = dynamic_cast<Facade*>(component.GetFacade());
		if (tmp)
		{
			window.draw(tmp->sprite);
		}		
	}
	window.display();
}

void Viewer::DrawShape(FShape shape, FTransform transform, FColor color)
{
	assert(shape.type == EShapeType::eBox);

	FVector extends = shape.extends;
	sf::Sprite sprite;
	sf::Texture texture;
	texture.loadFromFile("C:/Users/makde/Desktop/YetAnotherProject/box.png");
	sprite.setTexture(texture);

	sprite.setTextureRect(sf::IntRect(0, 0, 340, 320));
	sprite.setPosition(sf::Vector2f(15, 15));
	sprite.setRotation(0);
	window.draw(sprite);
}

Facade* Viewer::GetRootFacade()
{
	auto* sceneRoot = GetWorld()->GetSceneRoot();
	auto* ifacade   = sceneRoot->GetFacade();
	return dynamic_cast<Facade*>(ifacade);
}
