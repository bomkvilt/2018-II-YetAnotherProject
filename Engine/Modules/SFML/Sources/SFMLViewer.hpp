#ifndef SFML_VIEWER_HPP
#define SFML_VIEWER_HPP



#include "Interfaces/IViewer.hpp"
#include "Misc.hpp"
#include <memory>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFMLFacade.hpp"

class Viewer : public IViewer
{
public:
	Viewer(PlayerController* controller);

public:
	virtual void Render() override;
	virtual void DrawShape(FShape shape, FTransform transform, FColor color) override;

protected:
	Facade* GetRootFacade();

	sf::RenderWindow window;
};



#endif // !OSG_VIEWER_HPP
