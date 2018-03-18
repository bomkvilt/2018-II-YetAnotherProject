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
	setByInverseMatrix(osg::Matrix::inverse(matrix));
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

		osg::Matrixd mr, mt;
		mr.makeRotate(osg::Quat(
			static_cast<float>(rotation.v.x),
			static_cast<float>(rotation.v.y),
			static_cast<float>(rotation.v.z),
			static_cast<float>(rotation.w)
			));
		mt.makeTranslate(osg::Vec3(
			static_cast<float>(location.x),
			static_cast<float>(location.y),
			static_cast<float>(location.z)
			));


		lastMatrix = mr * mt;
	}
	return true;
}
