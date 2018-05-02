#include "OSGViewer.hpp"

#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/ViewerEventHandlers>
#include <osg/ShapeDrawable>


#include "World.hpp"
#include "ActorComponent.hpp"

#include "Control/EventHandlers.hpp"
#include "Camera/CameraManipulator.hpp"

#include "ComponentVisualisers.hpp"


Viewer::Viewer(PlayerController* controller, SHARED(FEngineConfig) config)
	: FViewer(controller, config)
	, window(nullptr)
{
	// setup windows
	window.reset(new osgWidget::WindowManager(
		&viewer, 
		config->viewer.ScreenSize.X,
		config->viewer.ScreenSize.Y
		));
	window->setPointerFocusMode(osgWidget::WindowManager::PFM_SLOPPY);
	window->setName(config->viewer.title);

	viewer.setUpViewInWindow(
		config->viewer.WindowPosition.X,
		config->viewer.WindowPosition.Y,
		static_cast<int>(window->getWidth ()),
		static_cast<int>(window->getHeight())
		);

	// setup scene
	Facade* facade = GetRootFacade();
	osg::Group* root = new osg::Group();
	osg::Camera* camera = window->createParentOrthoCamera();

	draw = osg::ref_ptr<osg::Group>(new osg::Group());


	root->addChild(draw);
	root->addChild(camera);
	root->addChild(facade->GetRoot());

	// add handlers
	viewer.addEventHandler     (new KeyboardHandler  (*GetEventCollector()) );
	viewer.addEventHandler     (new MouseHandler     (*GetEventCollector()) );
	viewer.setCameraManipulator(new CameraManipulator(*GetCameraManager ()) );

	viewer.addEventHandler(new osgWidget::ResizeHandler      (window.get(), camera));
	viewer.addEventHandler(new osgWidget::CameraSwitchHandler(window.get(), camera));
	viewer.addEventHandler(new osgViewer::WindowSizeHandler  (                    ));

	// update window
	window->resizeAllWindows();
	viewer.setSceneData(root);
	viewer.setReleaseContextAtEndOfFrameHint(false);

	if (!viewer.isRealized())
	{
		viewer.realize();
	}
}

void Viewer::Render()
{
	ClearDraw();
	Visualisate();
	viewer.frame();
}

void Viewer::DrawShape(FShape shape, FTransform transform, FColor color)
{
	using LBase  = osg::ref_ptr<osg::PositionAttitudeTransform>;
	using LShape = osg::ref_ptr<osg::ShapeDrawable>;
	auto MakeLShape = [](auto* shape) {
		return LShape(new osg::ShapeDrawable(shape));
		};

	osg::Vec3 origin(
		transform.Location.X,
		transform.Location.Y,
		transform.Location.Z
		);
	osg::Quat rot(
		transform.Rotation.X,
		transform.Rotation.Y,
		transform.Rotation.Z,
		transform.Rotation.W
		);
	osg::Vec3 len(
		shape.extents.X * 2,
		shape.extents.Y * 2,
		shape.extents.Z * 2
		);

	LShape mesh;
	switch (shape.type) {
	case EShapeType::eBox: mesh = MakeLShape( new osg::Box({0,0,0}, len.x(), len.y(), len.z()) ); break;
	default:
		throw std::out_of_range("shape.type has unsupported value");
	}

	if (mesh)
	{
		LBase base = new osg::PositionAttitudeTransform();
		base->setPosition(origin);
		base->setAttitude(rot);
		
		osg::Vec4 clr(
			color.R,
			color.G,
			color.B,
			color.A
			);
		mesh->setColor(clr);
		base->addChild(mesh);
		draw->addChild(base);
	}
}

Facade* Viewer::GetRootFacade()
{
	auto* sceneRoot = GetWorld()->GetSceneRoot();
	auto* ifacade   = sceneRoot->GetFacade();
	return static_cast<Facade*>(ifacade);
}

void Viewer::ClearDraw()
{
	auto total = draw->getNumChildren();
	draw->removeChildren(0, total);
}

void Viewer::Visualisate()
{
	auto& visualisersModule = ComponentVisualisersModule::Get();

	World& world = *GetWorld();
	for (auto& component : world)
	{
		visualisersModule.Visualise(&component, this);
	}
}
