# Main cmake list for CorEngine.

cmake_minimum_required (VERSION 3.8)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20) 
endif()

project("CorEngine" VERSION 0.1.4.2 LANGUAGES "CXX")

configure_file(${CMAKE_SOURCE_DIR}/backend/ver.cmake.in ${CMAKE_SOURCE_DIR}/backend/ver.hpp)

file(GLOB_RECURSE PUBLIC_SOURCE "src/*.cpp")
add_library(CorEngine STATIC ${PUBLIC_SOURCE})

file(GLOB_RECURSE PUBLIC_HEADERS "include/*.hpp")

target_include_directories(CorEngine PRIVATE 
"backend"
"include"
"include/auxiliary"
"include/engine"
"include/engine/audio"
"include/engine/core"
"include/engine/core/math"
"include/engine/core/windowing"
"include/engine/graphics"
"include/engine/input"
"include/engine/physics"
)

target_link_directories(
CorEngine
PUBLIC
"src/auxiliary"
"src/engine/audio"
"src/engine/core"
"src/engine/core/math"
"src/engine/core/windowing"
"src/engine/graphics"
"src/engine/input"
"src/engine/physics"
)

add_subdirectory(
"src/auxiliary"
)
add_subdirectory(
"src/engine/audio"
)
add_subdirectory(
"src/engine/core"
)
add_subdirectory(
"src/engine/core/math"
)
add_subdirectory(
"src/engine/core/windowing"
)
add_subdirectory(
"src/engine/graphics"
)
add_subdirectory(
"src/engine/input"
)
add_subdirectory(
"src/engine/physics"
)


# Vulkan
find_package(Vulkan REQUIRED) 

target_link_libraries(
	CorEngine
	PUBLIC

	Vulkan::Vulkan
	auxiliary
	audio
	core
	"math"
	windowing
	graphics
	"input"
	physics
)
