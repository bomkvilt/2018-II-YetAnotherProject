#ifndef OSG_VIEWER_HPP
#define OSG_VIEWER_HPP
#pragma once

#include <osgWidget/WindowManager>
#include <osgViewer/Viewer>

#include "Interfaces/Default/Viewer.hpp"
#include "OSGFacade.hpp"

class Viewer : public FViewer
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
