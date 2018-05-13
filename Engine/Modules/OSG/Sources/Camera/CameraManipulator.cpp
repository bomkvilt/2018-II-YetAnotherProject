#include "CameraManipulator.hpp"

#include "Components/CameraComponent.hpp"
#include "Camera/CameraManager.hpp"

#include <iostream>

CameraManipulator::CameraManipulator(CameraManager& manager)
	: manager(manager)
	, lastMatrix(osg::Matrix::identity())
{}

void CameraManipulator::setByMatrix(const osg::Matrixd& matrix)
{
	// Do Nothing - we can update location from an engine only!
}

void CameraManipulator::setByInverseMatrix(const osg::Matrixd& matrix)
{
	// Do Nothing - we can update location from an engine only!
}

osg::Matrixd CameraManipulator::getMatrix() const
{
	return lastMatrix;
}

osg::Matrixd CameraManipulator::getInverseMatrix() const
{
	return osg::Matrix::inverse(getMatrix());
}


bool CameraManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
{
	if (ea.getEventType() != osgGA::GUIEventAdapter::FRAME)
	{
		return false;
	}
	
	if (auto* camera = dynamic_cast<IWorldObject*>(manager.GetActiveCamera()))
	{
		const FQuat correction(0, -90, 0);

		auto worldTransform = camera->GetComponentTransform();
		auto rotation = worldTransform.Rotation * correction;
		auto location = worldTransform.Location;

		osg::Matrixd mr, mt;
		mr.makeRotate(osg::Quat(
			rotation.X,
			rotation.Y,
			rotation.Z,
			rotation.W
			));
		mt.makeTranslate(
			location.X,
			location.Y,
			location.Z
			);
		lastMatrix = mr * mt;
	}
	return true;
}
