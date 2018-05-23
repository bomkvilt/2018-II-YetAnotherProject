#include "SFMLFacade.hpp"
#include "BaseActorComponent.hpp"
#include "ResourceSystem.hpp"
#include "Modules/ModuleManager.hpp"


Facade::Facade(BaseActorComponent* owner, std::string relativePath)
	: IFacade(owner)
{
	auto& manager = ModuleManager::Get();
	auto& resources = manager.GetModule<ResourceSystem>();
	
	texture = resources.GetResource<RTexture>(relativePath);
	sprite.setTexture(texture->texture, true);

	auto rect = sprite.getTextureRect();
	rect.top += rect.height;
	rect.height *= -1;
	sprite.setTextureRect(rect);
}

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
	if (owner)
	{
		auto transform = owner->GetComponentTransform();

		FVector2 extents = owner->GetExtents();
		FVector2 origin  = transform(-extents);

		auto textureSize = texture->texture.getSize();
		auto scale = sf::Vector2f (
			extents.X * 2 / textureSize.x,
			extents.Y * 2 / textureSize.y
			);

		sprite.setRotation(0);
		sprite.setScale(scale);
		sprite.setPosition(sf::Vector2f(origin.X, origin.Y));
		sprite.rotate(transform.Rotation.GetEulerAngles().Z);
	}
}
