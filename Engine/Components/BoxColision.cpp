#include "BoxColision.hpp"


#include <osg/ShapeDrawable>


BoxColision::BoxColision()
{
	facade.mesh->addDrawable(new osg::ShapeDrawable(
		new osg::Box( osg::Vec3(0,0,0),1,1,1)
		));

	SetExtends({0.5, 0.5, 0.5});

	auto* node = facade.mesh->getChild(0);
	if (auto* shape = dynamic_cast<osg::ShapeDrawable*>(node))
	{
		shape->setColor(osg::Vec4(
			0, 0, 1, 0
		));
	}
}

void BoxColision::Tick(float DeltaTime, ETickType type)
{
	Super::Tick(DeltaTime, type);
}

void BoxColision::SetExtends(btVector3 newExtends)
{
	auto* node  = facade.mesh->getChild(0);

	if (auto* shape = dynamic_cast<osg::ShapeDrawable*>(node))
	{
		if (auto* box = dynamic_cast<osg::Box*>(shape->getShape()))
		{
			box->setHalfLengths(osg::Vec3(
				newExtends.x(),
				newExtends.y(),
				newExtends.z()
			));
		}
	}
	extends = newExtends;
}
