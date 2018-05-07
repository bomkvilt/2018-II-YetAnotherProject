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

	boxTexture.loadFromFile("../../box.png"); //TODO:: resource system
}

void Viewer::Render()
{
	window.clear();
	myMultimap.clear();
	sf::Event event;
	while (window.pollEvent(event))
	{
		keyboardHandler.handle(event);
		mouseHandler.handle(event);
	}
	auto& visualisersModule = ComponentVisualisersModule::Get();
	
	World& world = *GetWorld();
	for (auto& component : world)
	{
		visualisersModule.Visualise(&component, this);
		for (auto& pair : myMultimap)
			window.draw(pair.second);
		myMultimap.clear();
		if (auto* tmp = dynamic_cast<Facade*>(component.GetFacade()))
		{

		
			myMultimap.insert(std::make_pair(component.GetComponentTransform().Location.Z, tmp->sprite));
		}
		
	}
	
	for (auto& pair : myMultimap)
		window.draw(pair.second);
	
	window.display();
}

void Viewer::DrawShape(FShape shape, FTransform transform, FColor color)
{
	assert(shape.type == EShapeType::eBox);
	const int   pixinunit   = 80;
	const int   spritesize  = 510;
	const float scalefactor = (float)pixinunit/spritesize;
	
	FVector extents = shape.extents;
	
	sf::Sprite sprite;
	sprite.setTexture(boxTexture);
	sprite.setTextureRect(sf::IntRect(0, 0, spritesize, spritesize));
	
	FVector scale  = extents * scalefactor * 2;
	FVector origin = transform(-extents) * pixinunit + FVector(255, 255, 0);

	sprite.setScale(scale.X, scale.Y);
	sprite.setPosition(sf::Vector2f(origin.X, origin.Y));
	sprite.rotate(transform.Rotation.GetEulerAngles().Z);
	
	myMultimap.insert(std::make_pair(transform.Location.Z, sprite));
}

Facade* Viewer::GetRootFacade()
{
	auto* sceneRoot = GetWorld()->GetSceneRoot();
	auto* ifacade   = sceneRoot->GetFacade();
	return dynamic_cast<Facade*>(ifacade);
}
