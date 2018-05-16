#ifndef SFML_VIEWER_HPP
#define SFML_VIEWER_HPP


#include "Control/EventHandlers.hpp"
#include "Interfaces\Default\Viewer.hpp"
#include "Misc.hpp"
#include <memory>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFMLFacade.hpp"
#include <map>


class Viewer : public FViewer
{
public:
	Viewer(PlayerController* controller, SHARED(FEngineConfig) config);

public:
	virtual void Render() override;
	virtual void DrawShape(FShape shape, FTransform transform, FColor color) override;

protected:
	Facade* GetRootFacade();
	MouseHandler mouseHandler;
	KeyboardHandler keyboardHandler;
	sf::RenderWindow window;
	sf::Texture boxTexture;
	std::multimap <float, sf::Sprite> myMultimap;
	
};



#endif // !OSG_VIEWER_HPP
