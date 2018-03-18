cmake_minimum_required(VERSION 3.0)


macro(SetupEvniroment)

if(MSVC)
	#Use static libs
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}	/MT ")
	set(CMAKE_CXX_FLAGS_DEBUG	"${CMAKE_CXX_FLAGS_DEBUG}   /MTd")
	
	#Multiprocessing
	set(CMAKE_CXX_FLAGS			"${CMAKE_CXX_FLAGS}			/MP")
endif()

endmacro(SetupEvniroment)