#include "SFMLViewer.hpp"

#include "World.hpp"
#include "ResourceSystem.hpp"
#include "BaseActorComponent.hpp"
#include "Modules/ModuleManager.hpp"
#include "ComponentVisualisersModule.hpp"
#include "Components\CameraComponent.hpp"

#include <iostream>


Viewer::Viewer(PlayerController* controller, SHARED(FEngineConfig) config)
	: FViewer(controller, config)
	, keyboardHandler(*GetEventCollector())
	, mouseHandler   (*GetEventCollector())
{
	// init the window
	window.create(
		sf::VideoMode (
			static_cast<int>(config->frontend.ScreenSize.X), 
			static_cast<int>(config->frontend.ScreenSize.Y)
			), 
		config->frontend.title
		);
	window.setPosition(sf::Vector2i (
		static_cast<int>(config->frontend.WindowPosition.X),
		static_cast<int>(config->frontend.WindowPosition.Y)
		));

	// flip y axis
	sf::View view = window.getDefaultView();
	auto size = view.getSize();
	view.setSize(size.x, -size.y); 
	window.setView(view);

	// init resources
	auto& manager   = ModuleManager::Get();
	auto& resources = manager.GetModule<ResourceSystem>();
	emptyTexture    = resources.GetResource<RTexture>("empty.png");
}

void Viewer::Render()
{
	window    .clear();
	myMultimap.clear();

	sf::Event event;
	while (window.pollEvent(event))
	{
		keyboardHandler.handle(event);
		mouseHandler   .handle(event);
	}

	auto& visualisersModule = ComponentVisualisersModule::Get();
	
	World& world = *GetWorld();
	for (auto& component : world)
	{
		visualisersModule.Visualise(&component, this);

		if (auto* tmp = dynamic_cast<Facade*>(component.GetFacade()))
		{
			myMultimap.emplace(component.GetComponentTransform().Location.Z, GetAlligned(tmp->sprite));
		}
	}
	
	for (auto& pair : myMultimap)
	{
		window.draw(pair.second);
	}
	
	window.display();
}

void Viewer::DrawShape(FShape shape, FTransform transform, FColor color)
{
	assert(shape.type == EShapeType::eBox);

	if (emptyTexture)
	{
		FVector extents = shape.extents;
		FVector2 origin = transform(-extents);

		sf::Sprite sprite;
		sprite.setTexture(emptyTexture->texture);

		auto textureSize = emptyTexture->texture.getSize();
		auto scale = sf::Vector2f (
			extents.X * 2 / textureSize.x,
			extents.Y * 2 / textureSize.y
			);
		
		sprite.setScale(scale);
		sprite.setPosition(sf::Vector2f(origin.X, origin.Y));
		sprite.rotate(transform.Rotation.GetEulerAngles().Z);

		myMultimap.emplace(transform.Location.Z, GetAlligned(sprite));
	}
}

Facade* Viewer::GetRootFacade()
{
	auto* sceneRoot = GetWorld()->GetSceneRoot();
	auto* ifacade   = sceneRoot->GetFacade();
	return dynamic_cast<Facade*>(ifacade);
}

sf::Sprite Viewer::GetAlligned(sf::Sprite sprite)
{
	if (CameraComponent* cam = GetActiveCamera())
	{
		auto  windowSize = window.getSize();
		float renderSize = cam->GetRenderSize();
		float pixinunit  = windowSize.x / renderSize;

		sf::Vector2f camPosition (
			cam->GetComponentLocation().X,
			cam->GetComponentLocation().Y
			);
		
		auto rotation = -1        * ( sprite.getRotation()               );
		auto origin   = pixinunit * ( sprite.getPosition() - camPosition );
		auto scale    = pixinunit * ( sprite.getScale()                  );

		origin = sf::Vector2f (
			origin.x + static_cast<float>(windowSize.x) / 2,
			origin.y + static_cast<float>(windowSize.y) / 2
			);

		sprite.setPosition(origin);
		sprite.setScale   (scale );
	}

	return sprite;
}
