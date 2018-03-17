#include "PlayerController.hpp"

#include <assert.h>
#include <thread>

#include "World.hpp"
#include "Avatar.hpp"

#include "Control/EventHandlers.hpp"
#include "Control/CameraManipulator.hpp"

#include <osgGA/TerrainManipulator>


const unsigned int MASK_2D = 0xF0000000;
const unsigned int MASK_3D = 0x0F000000;



PlayerController::PlayerController()
	: world				(ThreadContextSingleton::Get().TopInitialiser()->world)
	, target			(nullptr)
	, window			(nullptr)
	, cameraManager		(viewer, world)
{	
	window.reset(new osgWidget::WindowManager(
		&viewer, 
		1280.0f, 
		1024.0f, 
		MASK_2D, 
		osgWidget::WindowManager::WM_PICK_DEBUG
		));

	window->setPointerFocusMode(osgWidget::WindowManager::PFM_SLOPPY);

	viewer.setUpViewInWindow(350, 150,
		static_cast<int>(window->getWidth()),
		static_cast<int>(window->getHeight())
		);

	auto* sceneRoot = world->GetSceneRoot();
	auto& facade    = sceneRoot->GetFacade();

	osg::Group*  group  = new osg::Group();
	osg::Camera* camera = window->createParentOrthoCamera();
	
	group->addChild(camera);
	group->addChild(facade.GetRoot());

	viewer.addEventHandler(new KeyboardHandler(eventCollector));
	viewer.addEventHandler(new MouseHandler	  (eventCollector));
	viewer.setCameraManipulator(new CameraManipulator(cameraManager));

	viewer.addEventHandler(new osgWidget::ResizeHandler			(window.get(), camera						));
	viewer.addEventHandler(new osgWidget::CameraSwitchHandler	(window.get(), camera						));
	viewer.addEventHandler(new osgViewer::WindowSizeHandler		(											));
	//viewer.addEventHandler(new osgViewer::StatsHandler			(											));
	//viewer.addEventHandler(new osgGA	::StateSetManipulator	(viewer.getCamera()->getOrCreateStateSet()	));
	
	//if (!viewer.getCameraManipulator()&& viewer.getCamera()->getAllowEventFocus())
	//{
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	//}
	
	window->resizeAllWindows();
	viewer.setSceneData(group);
	viewer.setReleaseContextAtEndOfFrameHint(false);

	if (!viewer.isRealized())
	{
		viewer.realize();
	}
}

PlayerController::~PlayerController()
{}

void PlayerController::OnBeginPlay()
{
	if (target)
	{
		EventBinder binder;

		target->SetupInput(&binder);

		eventManager.ProcessEventBinder(binder);
	}
}

void PlayerController::Tick(float DeltaTime, ETickType type)
{
	processInput();
}

void PlayerController::Render()
{
	viewer.frame();
}

void PlayerController::UnregisterCamera(CameraComponent* camera)
{
	cameraManager.UnregisterCamera(camera);
}

void PlayerController::RegisterCamera(CameraComponent* camera, bool bActivate)
{
	cameraManager.RegisterCamera(camera, bActivate);
}

void PlayerController::SetCameraActive(CameraComponent* camera, bool newState)
{
	cameraManager.SetCameraActive(camera, newState);
}

void PlayerController::processInput()
{
	auto& events  = eventCollector.GetEvents();
	float mouse_X = eventCollector.GetMouseX();
	float mouse_Y = eventCollector.GetMouseY();

	eventManager.ProcessKeys(events);
	eventManager.UpdateMouseLocation(mouse_X, mouse_Y);

	eventCollector.Flush();
}
