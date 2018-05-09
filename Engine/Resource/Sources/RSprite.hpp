#ifndef RSPRITE_HPP
#define RSPRITE_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Modules/IResource.hpp"
#include "Interfaces/IViewer.hpp"
#include "Misc.hpp"

#include "ComponentVisualiser.hpp"
#include "SFML\Graphics.hpp"
#include <string>


class RSprite : public IResource
{
	RSprite(std::string realtivePath);




private:
	sf::Sprite sprite;
};



#endif // !COMPONENT_VISUALISERS_HPP
