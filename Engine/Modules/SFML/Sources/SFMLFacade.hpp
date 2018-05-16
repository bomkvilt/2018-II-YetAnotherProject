#pragma once
#ifndef SFML_FACADE_HPP
#define SFML_FACADE_HPP

#include "Common.hpp"
#include "Interfaces/IFacade.hpp"
#include "SFML/Graphics.hpp"
#include "RTexture.hpp"



class Facade : public IFacade
{
public:
	Facade(BaseActorComponent* owner, std::string relativePath);

public:
	virtual bool AttachTo(IFacade* parent) override;
	virtual bool Detach() override;
	virtual void Update();

public:
	sf::Sprite       sprite;
	SHARED(RTexture) texture;
};



#endif // !SFML_FACADE_HPP