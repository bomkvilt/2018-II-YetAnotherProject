cmake_minimum_required(VERSION 3.0)

project(Pendulum
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
	)
	
add_executable(${PROJECT_NAME} "Main.cpp"
	"Pendulum.cpp"
	"Pendulum.hpp"
	"PendulumSymulationMode.cpp"
	"PendulumSymulationMode.hpp"
	"PendulumPlayerController.cpp"
	"PendulumPlayerController.hpp"
	)

include_directories(
	${OPENSCENEGRAPH_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS} 
	${BULLET_INCLUDE_DIRS}
	${VMATH_INCLUDE_DIRS}
	../Engine/
	)

target_link_libraries(${PROJECT_NAME}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES} 
	${BULLET_LIBRARIES}
	Engine
	)