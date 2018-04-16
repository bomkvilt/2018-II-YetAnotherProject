find_package(OpenGL			REQUIRED)
find_package(SFML 2 REQUIRED
	network 
	audio 
	graphics
	window 
	system )
	

DEFINE_UNIT(SFML)
SET(PUBLIC_INCLUDES
	${OPENSCENEGRAPH_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS} )
SET(PUBLIC_LIBRARIES
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES} )
SET(CODE		"Sources"	)
SET(TEST    	"Tests" 	)
SET(CATEGORY	"Engine/Modules"	)
SET(MODULES 	 Misc Core Components ComponentVisualisers )
MAKE_MODULE()
