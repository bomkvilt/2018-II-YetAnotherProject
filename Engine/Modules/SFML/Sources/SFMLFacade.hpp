#pragma once

#include <memory>

#include "Misc.hpp"
#include "Types.hpp"
#include "Interfaces/IFacade.hpp"
#include "SFML/Graphics.hpp"




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
	virtual void Update() override;
public:
	sf::Sprite sprite;
};