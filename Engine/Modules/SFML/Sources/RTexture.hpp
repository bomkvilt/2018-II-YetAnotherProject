#ifndef RSPRITE_HPP
#define RSPRITE_HPP

#include <memory>
#include <string>
#include "Interfaces/IResource.hpp"
#include "SFML/Graphics.hpp"



class RTexture : public IResource
{
public:
	RTexture(std::string relativePath)
		: IResource(relativePath)
	{
		texture.loadFromFile(relativePath); 	
	}

public:
	sf::Texture texture;
};



#endif // !COMPONENT_VISUALISERS_HPP
