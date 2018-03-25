#include "CameraManipulator.hpp"

#include "CameraManager.hpp"
#include "../../Components/CameraComponent.hpp"

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
	
	if (auto* camera = manager.GetCurrentCamera())
	{
		auto worldTransform = camera->GetComponentTransform();
		auto rotation = worldTransform.getRotation();
		auto location = worldTransform.getOrigin();

		auto v_rm = rotation.rotMatrix();

		osg::Matrixd mr, mt;

		mr.makeRotate(osg::Quat(
			rotation.v.x,
			rotation.v.y,
			rotation.v.z,
			rotation.w
			));
		mt.makeTranslate(
			location.x,
			location.y,
			location.z
			);
		lastMatrix = mr * mt;
	}
	return true;
}
