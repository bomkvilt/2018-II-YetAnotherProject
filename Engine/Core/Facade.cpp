
#include "Facade.hpp"


Facade::Facade()
	: rootOffset(new osg::PositionAttitudeTransform)
	, mesh		(new osg::Geode)
	, bHiden	(false)
{
	rootOffset->addChild(mesh);
}

Facade::~Facade()
{
}

void Facade::AttachTo(Facade& parent)
{
	parent.rootOffset->addChild(rootOffset);
}

void Facade::Detach()
{
	rootOffset->getParent(0)->removeChild(rootOffset);
}

osg::ref_ptr<osg::PositionAttitudeTransform> Facade::GetRoot()
{
	return rootOffset;
}
