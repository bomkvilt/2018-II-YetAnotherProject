#include "SFMLFacade.hpp"
#include "ActorComponent.hpp"


Facade::Facade(ActorComponent* owner)
	: IFacade(owner)
	, rootOffset(new osg::PositionAttitudeTransform)
	, mesh		(new osg::Geode)
	, bHiden	(false)
{
	rootOffset->addChild(mesh);
}

bool Facade::AttachTo(IFacade* parent)
{
	Facade* fparent = static_cast<Facade*>(parent);
	if (fparent)
	{
		fparent->rootOffset->addChild(rootOffset);
		return true;
	}
	return false;
}

bool Facade::Detach()
{
	if (rootOffset && rootOffset->getParent(0))
	{
		rootOffset->getParent(0)->removeChild(rootOffset);
		return true;
	}
	return false;
}

//inline void Facade::Update()
//{
//	if (!owner) return;
//	FTransform offset = owner->GetRelativeTransform();
//	FVector newVec    = offset.Location;
//
//	osg::Vec3 newPos = osg::Vec3(newVec.X, newVec.Y, newVec.Z);
//	GetRoot()->setPosition(newPos);
//
//	FQuat newQuat = offset.Rotation;
//	osg::Quat newRot = osg::Quat(
//		newQuat.X, 
//		newQuat.Y, 
//		newQuat.Z, 
//		newQuat.W);
//	GetRoot()->setAttitude(newRot);
//}
//
//osg::ref_ptr<osg::PositionAttitudeTransform> Facade::GetRoot()
//{
//	return rootOffset;
//}
