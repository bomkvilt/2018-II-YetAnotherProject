#include "SFMLViewer.hpp"




#include "World.hpp"
#include "ActorComponent.hpp"

#include "Control/EventHandlers.hpp"
#include "Camera/CameraManipulator.hpp"

#include "ComponentVisualisers.hpp"


const unsigned int MASK_2D = 0xF0000000;
const unsigned int MASK_3D = 0x0F000000;



Viewer::Viewer(PlayerController* controller)
	: IViewer(controller)
	, window(nullptr)
{
	// setup windows

	/*window.reset(new osgWidget::WindowManager(
		&viewer, 
		1280.0f, 
		1024.0f, 
		MASK_2D, 
		osgWidget::WindowManager::WM_PICK_DEBUG));
	window->setPointerFocusMode(osgWidget::WindowManager::PFM_SLOPPY);*/

	viewer.setUpViewInWindow(350, 150,
		static_cast<int>(window->getWidth()),
		static_cast<int>(window->getHeight())
		);

	// setup scene

	Facade* facade  = GetRootFacade();
	//osg::Group* root = new osg::Group();
	//osg::Camera* camera = window->createParentOrthoCamera();

	//draw = osg::ref_ptr<osg::Group>(new osg::Group());


	root->addChild(draw);
	root->addChild(camera);
	root->addChild(facade->GetRoot());

	// add handlers

	viewer.addEventHandler     (new KeyboardHandler  (*GetEventCollector()) );
	viewer.addEventHandler     (new MouseHandler	 (*GetEventCollector()) );
	viewer.setCameraManipulator(new CameraManipulator(*GetCameraManager ()) );

	//viewer.addEventHandler(new osgWidget::ResizeHandler			(window.get(), camera));
	//viewer.addEventHandler(new osgWidget::CameraSwitchHandler	(window.get(), camera));
	//viewer.addEventHandler(new osgViewer::WindowSizeHandler		(					 ));

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
	World& world = *GetWorld();
	for (auto& component : world)
	{
		ComponentVisualisers::Visualise(&component, this);
		auto* tmp = dynamic_cast<Facade*>(<ActorComponent*>.GetFacade());
		window.draw(tmp->sprite;);

		
	}
	viewer.frame();

}

void Viewer::DrawShape(FShape shape, FTransform transform, FColor color)
{

	//** create radius-vectors to the shape projection's vertices (Oxy)
	FVector extends = shape.extends;
	FVector vertices[4];

	// the sequence could be another.
	vertices[0] = { extends.X, extends.Y, 0 };
	vertices[1] = { extends.X, -extends.Y, 0 };
	vertices[2] = { -extends.X, -extends.Y, 0 };
	vertices[3] = { -extends.X, extends.Y, 0 };
	sf::RenderWindow window;
	for (auto& vert : vertices)
	{
		vert = transform * vert;
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sf::Texture texture;
	texture.update(image);
	sprite.setTextureRect(sf::IntRect(vertices[2].X, vertices[3].Y, vertices[2].X, vertices[1].X*2, vertices[0].Y * 2));
	
	
	window.Draw(sprite);
	//** here we need to crate a sfml sprite and place the vertices into the one (X and Y components)

	//** here u must place a default texture into the sprite

	//** and here - call another function to draw the sprite on a canvas
	/*using LBase  = osg::ref_ptr<osg::PositionAttitudeTransform>;
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
		shape.extends.X * 2,
		shape.extends.Y * 2,
		shape.extends.Z * 2
		);

	LShape mesh;
	switch (shape.type) {
	case EShapeType::eBox: mesh = MakeLShape( new osg::Box({0,0,0}, len.x(), len.y(), len.z()) );
	// TODO:
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
	}*/
}

Facade* Viewer::GetRootFacade()
{
	auto* sceneRoot = GetWorld()->GetSceneRoot();
	auto* ifacade   = sceneRoot->GetFacade();
	return dynamic_cast<Facade*>(ifacade);
}

void Viewer::ClearDraw()
{
	auto total = draw->getNumChildren();
	draw->removeChildren(0, total);
}

void Viewer::Visualisate()
{
	World& world = *GetWorld();
	for (auto& component : world)
	{
		ComponentVisualisers::Visualise(&component, this);
	}
}
