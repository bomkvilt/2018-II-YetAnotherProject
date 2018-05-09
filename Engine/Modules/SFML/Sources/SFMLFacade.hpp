#pragma once

#include <memory>

#include "Misc.hpp"
#include "Types.hpp"
#include "Interfaces/IFacade.hpp"
#include "SFML/Graphics.hpp"
#include "RTexture.hpp"



/** Facade - is abstract visualisation component
 *	generateing a 
 */
class Facade : public IFacade
{
public:
	Facade(ActorComponent* owner);

public:

	virtual bool AttachTo(IFacade* parent) override;
	virtual bool Detach() override;
	virtual void Update();
	virtual void SetupPathToSprite(std::string path);
public:
	sf::Sprite sprite;
	SHARED(RTexture) texture;
};