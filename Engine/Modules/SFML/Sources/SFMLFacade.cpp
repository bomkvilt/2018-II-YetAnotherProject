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
		FVector2 pixinunit(80, 80);
		auto textureSize  = texture->texture.getSize();
		auto textureSize2 = FVector2(textureSize.x, textureSize.y);
		auto transform   = owner->GetComponentTransform();

		FVector2 extents = owner->GetExtents();
		FVector2 scalefactor = pixinunit * extents * 2 / textureSize2;

		FVector origin  = transform(-extents) * pixinunit + FVector2(255, 255);

		sprite.setRotation(0);
		sprite.setPosition(sf::Vector2f(origin.X, origin.Y));
		sprite.rotate(transform.Rotation.GetEulerAngles().Z);
		sprite.setScale(scalefactor.X, scalefactor.Y);
	}
}
