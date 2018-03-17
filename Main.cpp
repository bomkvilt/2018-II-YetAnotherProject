
#include <stdio.h>

#include <memory>

#include "btBulletDynamicsCommon.h"


int Px() {
	
	///-----initialization_start-----

	// colision
	auto collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>	();
	auto dispatcher				= std::make_unique<btCollisionDispatcher> 
	(
		collisionConfiguration.get()
	);
	auto overlappingPairCache	= std::make_unique<btDbvtBroadphase>();
	auto solver					= std::make_unique<btSequentialImpulseConstraintSolver>();
	auto dynamicsWorld			= std::make_unique<btDiscreteDynamicsWorld>
	(
		dispatcher.get(), 
		overlappingPairCache.get(), 
		solver.get(), 
		collisionConfiguration.get()
	);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///-----initialization_end-----

	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	///create a few basic rigid bodies

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}

	/// Do some simulation

	///-----stepsimulation_start-----
	for (int i = 0; i < 150; i++)
	{
		dynamicsWorld->stepSimulation(1.f / 60.f, 10);

		//print positions of all objects
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
			printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}
	}

	///-----stepsimulation_end-----

	//cleanup in the reverse order of creation/initialization

	///-----cleanup_start-----

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();

	return 0;
}






#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>

#include <osgViewer/Viewer>
#include <iostream>
#include <osgDB/ReadFile>
#include <osg/Math>
#include <osg/Matrixd>
#include <osg/MatrixTransform>
#include <osg/Vec3d>
#include <iostream>
#include <osg/PositionAttitudeTransform>



float jointAngle1;
float jointAngle2;
float EndEffector;

void createAxis(osg::Transform* previousJoint);

osg::MatrixTransform* joint1;
osg::MatrixTransform* joint2;

osg::MatrixTransform* buildJoint1	(osg::MatrixTransform* previousJoint);
osg::MatrixTransform* buildJoint2	(osg::MatrixTransform* previousJoint);
osg::MatrixTransform* buildEndEffector();

osg::TessellationHints* hints = new osg::TessellationHints;

bool showAxis;

class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:

	KeyboardEventHandler()
	{}


	static void rotate(float angle,osg::MatrixTransform *joint)
	{
		osg::Matrix zRot;
		zRot.makeRotate(angle, 0.0,0.0,1.0);
		joint->setMatrix(zRot*joint->getMatrix());
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
	{
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			switch (ea.getKey()) {
			case 'q':	rotate( osg::PI/180, joint1);	return true;
			case 'a':	rotate(-osg::PI/180, joint1);	return true;
			case 'w':	rotate( osg::PI/180, joint2);	return true;
			case 's':	rotate(-osg::PI/180, joint2);	return true;
			}
		}
		default:
			break;
		}
		//return false to allow mouse manipulation
		return false;
	}
};


osg::Group* createShapes()
{
	osg::Group* group = new osg::Group();
	osg::MatrixTransform* transform =new osg::MatrixTransform();
	group->addChild(transform);

	joint1 = buildJoint1(transform);
	joint2 = buildJoint2(joint1);
	joint2->addChild( buildEndEffector());
	return group;
}

osg::MatrixTransform* buildJoint1(osg::MatrixTransform* previousJoint)
{
	osg::MatrixTransform* xTransform =new osg::MatrixTransform();
	previousJoint->addChild(xTransform);
	float radius = 6.7640f;
	float height = 45.0f;
	osg::Geode* joint = new osg::Geode();
	xTransform->addChild(joint);
	joint->addDrawable(
		new osg::ShapeDrawable(
			new osg::Cylinder(
				osg::Vec3(0.0f,0.0f,height/2),radius,height
			),
			hints
		)
	);

	osg::MatrixTransform *zTransform = new osg::MatrixTransform();
	xTransform->addChild(zTransform);
	osg::Matrix zTrans=osg::Matrix::translate(0.0, 0.0, height);
	osg::Matrix zRot= osg::Matrix::rotate(jointAngle1, 0.0, 0.0, 1.0);
	zTransform->setMatrix(zTrans*zRot);
	return zTransform;
}

osg::MatrixTransform* buildJoint2(osg::MatrixTransform* previousJoint)
{
	if (showAxis)
	{
		createAxis(previousJoint);
	}
	double height = 17.6;
	double radius = 4.45633;
	osg::MatrixTransform* xTransform = new osg::MatrixTransform();
	previousJoint->addChild(xTransform);
	osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
	xTransform->setMatrix(xRot);
	osg::Geode* joint = new osg::Geode();
	joint->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(0.0f,0.0f,height/2),radius,height),hints));
	xTransform->addChild(joint);

	osg::MatrixTransform* zTransform = new osg::MatrixTransform();
	osg::Matrix zTrans = osg::Matrix::translate( 0.0, 0.0, height);
	osg::Matrix zRot = osg::Matrix::rotate(osg::PI_2+jointAngle2, 0.0,0.0,1.0);
	zTransform->setMatrix(zTrans*zRot);
	xTransform->addChild(zTransform);
	return zTransform;
}

osg::MatrixTransform* buildEndEffector()
{
	osg::MatrixTransform* mt = new osg::MatrixTransform();
	osg::Matrix m;
	double length = 17.0;
	m.makeTranslate(0,0,length/2);
	mt->setMatrix(m);

	osg::Geode *geode_3 = new osg::Geode;
	osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-EndEffector, 0.0f, 0.0f), .5, 1.5f, length), hints);
	osg::ShapeDrawable *shape2 = new osg::ShapeDrawable(new osg::Box(osg::Vec3( EndEffector, 0.0f, 0.0f), .5, 1.5f, length), hints);
	shape1->setColor(osg::Vec4(0.8f, 0.8f, 0.4f, 1.0f));
	shape2->setColor(osg::Vec4(0.8f, 0.8f, 0.4f, 1.0f));
	geode_3->addDrawable(shape1);
	geode_3->addDrawable(shape2);
	mt->addChild(geode_3);
	return mt;
}



void createAxis(osg::Transform* previousJoint)
{
	double height = 12.0;
	double radius = .5;

	osg::MatrixTransform* zmt = new osg::MatrixTransform();

	previousJoint->addChild(zmt);
	osg::ShapeDrawable *zShape = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f,0.0f,height/2),radius,height),hints);
	osg::ShapeDrawable *zCone = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.0f,0.0f,1.0),radius+1.0,2.0),hints);

	osg::MatrixTransform* zmtCone = new osg::MatrixTransform();
	osg::Geode *zgCone = new osg::Geode;

	zmtCone->setMatrix( osg::Matrix::translate(0.0,0.0,height));
	previousJoint->addChild(zmtCone);

	zShape->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	zCone->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	osg::Geode *z = new osg::Geode;
	z->addDrawable(zShape);
	zgCone->addDrawable(zCone);
	zmtCone->addChild(zgCone);
	zmt->addChild(z);

	osg::MatrixTransform* mt = new osg::MatrixTransform();
	previousJoint->addChild(mt);

	osg::Matrix xMatrix = osg::Matrix::rotate(-osg::PI_2, 0.0, 1.0, 0.0);
	mt->setMatrix(xMatrix);


	osg::ShapeDrawable *xShape = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f,0.0f,height/2),radius,height),hints);
	xShape->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	osg::Geode *x = new osg::Geode;
	x->addDrawable(xShape);
	mt->addChild(x);


	osg::MatrixTransform *yMt = new osg::MatrixTransform();
	previousJoint->addChild(yMt);
	osg::Matrix yMatrix = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
	yMt->setMatrix(yMatrix);

	osg::ShapeDrawable *yShape = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f,0.0f,height/2),radius,height),hints);
	yShape->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	osg::Geode *y = new osg::Geode;
	y->addDrawable(yShape);
	yMt->addChild(y);
}

int main(int, char **)
{
	hints->setDetailRatio(0.5f);
	showAxis = true;
	
	jointAngle1=0.0;
	jointAngle2=0.0;
	EndEffector=1.0;

	osgViewer::Viewer viewer;
	viewer.addEventHandler(new KeyboardEventHandler());

	// add model to viewer.
	viewer.setSceneData( createShapes() );
	return viewer.run();
}
