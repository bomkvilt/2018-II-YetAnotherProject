
# add unit's sources/headers to the project
#
MACRO(__SETUP_SOURCE)
	SET(SOURCES_DIRS ${CODE})
	SET(HEADERS_DIRS ${CODE})
	
	# find project files
	file(GLOB_RECURSE SOURCES "${SOURCES_DIRS}/*.cpp")
	file(GLOB_RECURSE HEADERS "${HEADERS_DIRS}/*.hpp")
	
	# add the files into project's code
	source_group(TREE ${PROJECT_SOURCE_DIR}/${SOURCES_DIRS} PREFIX "Source Files" FILES ${SOURCES})
	source_group(TREE ${PROJECT_SOURCE_DIR}/${HEADERS_DIRS} PREFIX "Header Files" FILES ${HEADERS})
ENDMACRO()

# add required includes (from other modules and 3rdParty)
#
MACRO(__SETUP_INCLUDES)
	# get public includes from assigned units
	FOREACH(module ${MODULES})
		get_property(unit_DIR GLOBAL PROPERTY ${module}_DIR)
		list(APPEND MODULE_INCLUDES ${unit_DIR}) 
	ENDFOREACH()
	
	# combine project includes
	set(ALL_INCLUDES ${HEADERS_DIRS} ${INCLUDES} ${PUBLIC_INCLUDES} ${MODULE_INCLUDES})
	
	# add the includes into the project
	include_directories(${ALL_INCLUDES})
ENDMACRO()

# link required libs
#
MACRO(__SETUP_LINKS)
	# get public libs from assigned modules
	FOREACH(module ${MODULES})
		get_property(unit_LIB GLOBAL PROPERTY ${module}_LIB)
		list(APPEND MODULE_LIBS ${unit_LIB})
	ENDFOREACH()
	
	# combine project links
	SET(ALL_LINKS ${MODULE_LIBS} ${LIBRARIES} ${PUBLIC_LIBRARIES})
	
	# add the libs into the project
	target_link_libraries(${TARGET} ${ALL_LINKS})
ENDMACRO()

# set project's folder in  vs project
#
MACRO(__SETUP_FOLDER)
	SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER "${CATEGORY}")
ENDMACRO()

# setup project properties
#
MACRO(__SETUP_PROPERTIES)
	SET_TARGET_PROPERTIES(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${ENGINE_ROOT})
ENDMACRO()

# add module meta into a global space
#
MACRO(__DEFINE_MODULE)
	get_filename_component(PB_HEADERS_DIRS    "${HEADERS_DIRS}"    ABSOLUTE)
	get_filename_component(PB_PUBLIC_INCLUDES "${PUBLIC_INCLUDES}" ABSOLUTE)

	SET(TARGET_PUBLIC_DIRS ${PB_HEADERS_DIRS} ${PB_PUBLIC_INCLUDES})
	SET(TARGET_PUBLIC_LIBS ${TARGET}          ${PUBLIC_LIBRARIES}  )

	set_property(GLOBAL PROPERTY ${TARGET}_DIR ${TARGET_PUBLIC_DIRS})
	set_property(GLOBAL PROPERTY ${TARGET}_LIB ${TARGET_PUBLIC_LIBS})
ENDMACRO()

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MACRO(INIT_PROJECT ENGINE PROJECT)
	SET(ENGINE_ROOT ${PROJECT_SOURCE_DIR})

	SUBDIRS(${ENGINE} )
	SUBDIRS(${PROJECT})
ENDMACRO()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# define new unit (module / application)
#
MACRO(DEFINE_UNIT NAME)
	SET(TARGET ${NAME})		# unit name
	project(${TARGET})		# create a cmake project
	__DEFINE_MODULE()		# add the unit's meta into a global space
	
	#-------------------------------------------------# interface variables
	SET(MODULES)			# required modules
	SET(CATEGORY)			# project folder in a solution	
	SET(CODE)				# directory with project code
	SET(TEST)				# directory with project tests
	
	SET(INCLUDES)			# include files from 3rdParty
	SET(LIBRARIES)			# .lib    files from 3tdParty	
	SET(PUBLIC_INCLUDES)	# include files from 3rdParty
	SET(PUBLIC_LIBRARIES)	# .lib 	  files from 3tdParty
	
	#-------------------------------------------------# internal variables
	SET(HEADERS_DIRS)		# found includes for the unit
	SET(SOURCES_DIRS)		# found sources  for the unit
	
	# NOTE:
	# PUBLIC_*		will be also exported to up linking modules
ENDMACRO()


# use the unit as an module
#
MACRO(MAKE_MODULE)
	__SETUP_SOURCE()
	__SETUP_INCLUDES()
	add_library(${TARGET} 
		${HEADERS} 
		${SOURCES}
		${INCLUDES}
		)
	__SETUP_FOLDER()
	__DEFINE_MODULE()
ENDMACRO()


# use the unit as an application
#
MACRO(MAKE_APP)
	__SETUP_SOURCE()
	__SETUP_INCLUDES()
	add_executable(${TARGET} 
		${HEADERS} 
		${SOURCES}
		${INCLUDES}
		)
	__SETUP_PROPERTIES()
	__SETUP_LINKS()
	__SETUP_FOLDER()
	__DEFINE_MODULE()
ENDMACRO()
