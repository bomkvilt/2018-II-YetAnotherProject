#ifndef OSG_VIEWER_HPP
#define OSG_VIEWER_HPP

#include <osgWidget/WindowManager>
#include <osgViewer/Viewer>

#include "Interfaces/IViewer.hpp"
#include "Misc.hpp"
#include <memory>

#include "OSGFacade.hpp"

class Viewer : public IViewer
{
public:
	Viewer(PlayerController* controller, SHARED(FEngineConfig) config);

public:
	virtual void Render() override;
	virtual void DrawShape(FShape shape, FTransform transform, FColor color) override;

protected:
	Facade* GetRootFacade();
	
	void ClearDraw();
	void Visualisate();

	/// >> layers
	osg::ref_ptr<osg::Group> draw;
	/// << 

	/// >> window
	osgViewer::Viewer				 viewer;
	UNIQUE(osgWidget::WindowManager) window;
	///
};



#endif // !OSG_VIEWER_HPP
