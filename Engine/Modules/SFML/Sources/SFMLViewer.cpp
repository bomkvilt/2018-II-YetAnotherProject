#include "SFMLViewer.hpp"

#include "World.hpp"
#include "ActorComponent.hpp"
#include "ComponentVisualisersModule.hpp"
#include "CameraComponent.hpp"
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
	CameraComponent* cam = GetActiveCamera();
	assert(cam != nullptr);
		FVector renderSize = cam->GetRenderSize();

		const sf::Vector2u  pixinunit(
			window.getSize().x / renderSize.X,
			window.getSize().y / renderSize.Y
		);
		

		const sf::Vector2u  spritesize = boxTexture.getSize();
		const sf::Vector2f scalefactor(
			static_cast<float>(pixinunit.x) / static_cast<float>(spritesize.x),
			static_cast<float>(pixinunit.y) / static_cast<float>(spritesize.y)
		);
			

	
	FVector extents = shape.extents;
	
	sf::Sprite sprite;
	sprite.setTexture(boxTexture);
	sprite.setTextureRect(sf::IntRect(0, 0, spritesize.x, spritesize.y));
	
	FVector scale  (
		extents.X * scalefactor.x * 2,
		extents.Y * scalefactor.y * 2,
		0

	);
	FVector origin(
		transform(-extents).X * pixinunit.x,
		transform(-extents).Y * pixinunit.y,
		0);
		
		
		
	origin+= FVector(255, 255, 0);

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
