#ifndef RSPRITE_HPP
#define RSPRITE_HPP

#include <memory>
#include <string>
#include "Modules/IResource.hpp"
#include "SFML\Graphics.hpp"



class RTexture : public IResource
{
	RTexture(std::string relativePath):
		IResource(relativePath)
	{
		texture.loadFromFile(relativePath); 
		
	};




protected:
	
	sf::Texture texture;
};



#endif // !COMPONENT_VISUALISERS_HPP
