cmake_minimum_required(VERSION 3.0)

project(PendulumEngine
	VERSION 0.1
	LANGUAGES CXX)
	
include("Cmake/DefaultSetup.cmake")
SetupEvniroment()
	
add_subdirectory(Engine)
add_subdirectory(Pendulum)

find_package(Bullet 		REQUIRED)
find_package(OpenGL			REQUIRED)
find_package(OpenSceneGraph REQUIRED
	osgViewer
	osgWidget
	osgDB
	osgGA
	)
	
add_executable(${PROJECT_NAME} "main.cpp")

include_directories(
	${OPENSCENEGRAPH_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS} 
	${BULLET_INCLUDE_DIRS}
	Engine/
	Pendulum/
	)

target_link_libraries(${PROJECT_NAME}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES} 
	${BULLET_LIBRARIES}
	Engine
	)