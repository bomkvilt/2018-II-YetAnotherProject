cmake_minimum_required(VERSION 3.10)

project(Engine
	VERSION 0.1
	LANGUAGES CXX)

set(3rdParty "../3rdParty")

include("../Cmake/DefaultSetup.cmake")
include("../Cmake/VMath.cmake")
SetupEvniroment()
DefineVMath()
	
find_package(Bullet 		REQUIRED)
find_package(OpenGL			REQUIRED)
find_package(OpenSceneGraph REQUIRED
	osgViewer
	osgWidget
	osgDB
	osgGA
	)
	
include("../Cmake/DefaultSetup.cmake")
SetupEvniroment()
	
file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADERS *.hpp)

source_group(TREE ${PROJECT_SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCES})
source_group(TREE ${PROJECT_SOURCE_DIR} PREFIX "Header Files" FILES ${HEADERS})

include_directories(${HEADERS} 
	${OPENSCENEGRAPH_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS} 
	${BULLET_INCLUDE_DIRS}
	${VMATH_INCLUDE_DIRS}
	)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES} 
	${BULLET_LIBRARIES}
	)