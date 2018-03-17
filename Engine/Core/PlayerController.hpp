#pragma once

#include <memory>
#include <iostream>

#include "../Misc.hpp"
#include "../Types.hpp"

#include "Control/EventManager.hpp"
#include "Control/EventCollector.hpp"
#include "Control/CameraManager.hpp"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/Math>
#include <osg/Matrixd>
#include <osg/MatrixTransform>
#include <osg/Vec3d>
#include <osg/PositionAttitudeTransform>

#include <osgWidget/WindowManager>
#include <osgWidget/Box>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgDB/FileUtils>
#include <osgDB/WriteFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/Util>
#include <osgWidget/ViewerEventHandlers>
#include <osgWidget/WindowManager>


class World;
class Avatar;
class CameraComponent;


/** The class controls an assigned avatars
 */
class PlayerController
{
public:

	PlayerController();
	virtual ~PlayerController();

public:

	virtual void OnBeginPlay();


	virtual void Tick(float DeltaTime, ETickType type);
	virtual void Render();

public:	//~~~~~~~~~~~~~~~~~~~| Common

	const World* GetWorld() const		{ return world;			}
		  World* GetWorld()				{ return world;			}
	void SetWorld(World* newWorld)		{ world = newWorld;		}

	const Avatar* GetTarget() const		{ return target;		}
		  Avatar* GetTarget()			{ return target;		}
	void SetTarget(Avatar* newTarget)	{ target = newTarget;	}

public: //~~~~~~~~~~~~~~~~~~~| Camera

	void UnregisterCamera(CameraComponent* camera);
	void RegisterCamera	 (CameraComponent* camera, bool bActivate = true);
	void SetCameraActive (CameraComponent* camera, bool newState  = true);

protected:

	void processInput();

protected:

	/// >> common
	World*  world;
	Avatar* target;
	/// << 

	/// >> camera
	CameraManager cameraManager;
	/// <<

	/// >> control
	EventCollecor eventCollector;
	EventManager  eventManager;
	/// <<

	/// >> window
	osgViewer::Viewer viewer;

	UNIQUE(osgWidget::WindowManager) window;
	///

};