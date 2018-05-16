set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_ROOT ${3rdParty}/SFML)

find_package(OpenGL REQUIRED)
find_package(SFML 2 REQUIRED
	graphics
	network 
	window 
	system 
	audio )
	
DEFINE_UNIT(SFML)
SET(PUBLIC_INCLUDES
	${SFML_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS} )
SET(PUBLIC_LIBRARIES
	${SFML_LIBRARIES}
	${OPENGL_LIBRARIES} 
	${SFML_DEPENDENCIES} )
SET(CODE		"Sources"	)
SET(TEST    	"Tests" 	)
SET(CATEGORY	"Engine/Modules"	)
SET(MODULES 	 Misc Core Components GameFramwork ComponentVisualisers Resources)
MAKE_MODULE()
