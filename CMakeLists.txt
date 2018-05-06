cmake_minimum_required(VERSION 3.0)

project(YetAnotherProject)

#~~~~~~~~~~~~~~~~~~~~~~~| >> setup project | 
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
INCLUDE("Cmake/DefaultSetup.cmake")
INCLUDE("Cmake/MacroUtiles.cmake")

SET(3rdParty "${CMAKE_SOURCE_DIR}/3rdParty")
SetupEvniroment()
	
#~~~~~~~~~~~~~~~~~~~~~~~| >> init project |
INIT_PROJECT(Engine Pendulum)

#~~~~~~~~~~~~~~~~~~~~~~~| >> make test module
#MAKE_MODULE(PendulumEngine)
#find_package(Bullet 		REQUIRED)
#SET(INCLUDES  ${BULLET_INCLUDE_DIRS})
#SET(LIBRARIES ${BULLET_LIBRARIES}   )
#SET(CATEGORY "Project")
#SET(SOURCES "main.cpp")
#SETUP_APP()