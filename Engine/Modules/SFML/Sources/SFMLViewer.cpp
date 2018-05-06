#include "SFMLViewer.hpp"

#include "World.hpp"
#include "ActorComponent.hpp"
#include "ComponentVisualisersModule.hpp"

#include <iostream>


Viewer::Viewer(PlayerController* controller, SHARED(FEngineConfig) config)
	: FViewer(controller, config),
	keyboardHandler(*GetEventCollector()),
	mouseHandler(*GetEventCollector())
{
	window.create(sf::VideoMode(1024, 768), "test");
}

void Viewer::Render()
{
	window.clear();

	sf::Event event;
	while(window.pollEvent(event))
	{
		keyboardHandler.handle(event);
		mouseHandler.handle(event);
	}
	auto& visualisersModule = ComponentVisualisersModule::Get();


	World& world = *GetWorld();
	for (auto& component : world)
	{
		visualisersModule.Visualise(&component, this);
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
	const int   pixinunit   = 80;
	const int   spritesize  = 510;
	const float scalefactor = (float)pixinunit/spritesize;
//	std::cout << scalefactor << std::endl;
	
	FVector extents = shape.extents;
	
	sf::Sprite sprite;
	sf::Texture texture;
	texture.loadFromFile("../../box.png"); //TODO:: resource system
	sprite.setTexture(texture);
	
	sprite.setTextureRect(sf::IntRect(0, 0, spritesize, spritesize));
	
	FVector scale  = extents * scalefactor * 2;
	FVector origin = transform(-extents) * pixinunit + FVector(255, 255, 0);

	sprite.setScale(scale.X, scale.Y);
	sprite.setPosition(sf::Vector2f(origin.X, origin.Y));
	sprite.rotate(transform.Rotation.GetEulerAngles().Z);
	
	window.draw(sprite);
}

Facade* Viewer::GetRootFacade()
{
	auto* sceneRoot = GetWorld()->GetSceneRoot();
	auto* ifacade   = sceneRoot->GetFacade();
	return dynamic_cast<Facade*>(ifacade);
}
