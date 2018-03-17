#pragma once

#include <memory>

#include "../Misc.hpp"


#include <osg/ref_ptr>
#include <osg/Switch>
#include <osg/Geode>
#include <osg/Math>
#include <osg/Matrixd>
#include <osg/MatrixTransform>
#include <osg/Vec3d>
#include <osg/PositionAttitudeTransform>



/** Facade - is abstract visualisation component
 *	generateing a 
 */
class Facade
{
public:

	Facade();
	virtual ~Facade();

public:

	void AttachTo(Facade& parent);
	void Detach();

public:

	osg::ref_ptr<osg::PositionAttitudeTransform> GetRoot();

public:

	/// >>
	osg::ref_ptr<osg::PositionAttitudeTransform> rootOffset;
	osg::ref_ptr<osg::Geode					   > mesh;

	bool bHiden;
	/// <<
};