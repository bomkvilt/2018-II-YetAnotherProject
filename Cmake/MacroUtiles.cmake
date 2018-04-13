
MACRO(__SETUP_SOURCE)
	source_group(TREE ${PROJECT_SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCES})
	source_group(TREE ${PROJECT_SOURCE_DIR} PREFIX "Header Files" FILES ${HEADERS})
ENDMACRO()

MACRO(__SETUP_INCLUDES)
	FOREACH(module ${MODULES})
		get_property(module_DIR GLOBAL PROPERTY ${module}_DIR)
		list(APPEND MODULES_INCLUDE_DIRS ${module_DIR}) 
	ENDFOREACH()
	list(APPEND INCLUDES ${MODULES_INCLUDE_DIRS})
	
	include_directories(${HEADERS} ${INCLUDES})
ENDMACRO()

MACRO(__SETUP_LINKS)
	FOREACH(module ${MODULES})
		list(APPEND MODULES_LINKS ${module})
	ENDFOREACH()
	list(APPEND LINKS ${MODULES_LINKS})
	list(APPEND LINKS ${LIBRARIES})
	target_link_libraries(${TARGET} ${LINKS})
ENDMACRO()

MACRO(__SETUP_FOLDER)
	SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER "${CATEGORY}")
ENDMACRO()

MACRO(__DEFINE_MODULE)
	set_property(GLOBAL PROPERTY ${TARGET}_DIR ${PROJECT_SOURCE_DIR})
ENDMACRO()

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MACRO(INIT_PROJECT ENGINE PROJECT)
	SUBDIRS(${ENGINE} )
	SUBDIRS(${PROJECT})
ENDMACRO()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MACRO(MAKE_MODULE NAME)
	SET(TARGET ${NAME})	
	project(${TARGET})
	__DEFINE_MODULE()
	SET(MODULES)
	SET(HEADERS)
	SET(SOURCES)
	SET(INCLUDES)
	SET(CATEGORY)
ENDMACRO()

MACRO(SETUP_MODULE)
	__SETUP_SOURCE()
	__SETUP_INCLUDES()
	add_library(${TARGET} 
		${HEADERS} 
		${SOURCES}
		${INCLUDES}
		)
	__SETUP_LINKS()
	__SETUP_FOLDER()
ENDMACRO()

MACRO(SETUP_APP)
	__SETUP_SOURCE()
	__SETUP_INCLUDES()
	add_executable(${TARGET} 
		${HEADERS} 
		${SOURCES}
		${INCLUDES}
		)
	__SETUP_LINKS()
	__SETUP_FOLDER()
ENDMACRO()