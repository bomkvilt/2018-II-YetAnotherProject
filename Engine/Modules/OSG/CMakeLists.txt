find_package(OpenGL			REQUIRED)
find_package(OpenSceneGraph REQUIRED
	osgViewer
	osgWidget
	osgDB
	osgGA )
	

DEFINE_UNIT(OSG)
SET(PUBLIC_INCLUDES
	${OPENSCENEGRAPH_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS} )
SET(PUBLIC_LIBRARIES
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES} )
SET(CODE		"Sources"	)
SET(TEST    	"Tests" 	)
SET(CATEGORY	"Engine/Modules"	)
SET(MODULES 	 Misc Core Components ComponentVisualisers GameFramwork)
MAKE_MODULE()
