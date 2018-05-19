#ifndef CAMERA_MANIPULATOR_HPP
#define CAMERA_MANIPULATOR_HPP
#pragma once

#include <osgGA/CameraManipulator>

class CameraManager;


/** CameraManipulator - provides a camera coordinates from 
 *	attached camera manager to 'inreface' methods
 */
class CameraManipulator : public osgGA::CameraManipulator
{
public:

	CameraManipulator(CameraManager& manager);

	/// >> CameraManipulator - interface
	virtual void setByMatrix	    (const osg::Matrixd& matrix)	override;
	virtual void setByInverseMatrix (const osg::Matrixd& matrix)	override;

	virtual osg::Matrixd getMatrix			() const override;
	virtual osg::Matrixd getInverseMatrix	() const override;
	/// << 

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&) override;

protected:

	CameraManager& manager;

	osg::Matrix lastMatrix;
};



#endif // ! CAMERA_MANIPULATOR_HPP
