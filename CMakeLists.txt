# Main cmake list for CorEngine.

cmake_minimum_required (VERSION 3.8)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20) 
endif()

project("CorEngine" VERSION 0.1.4.3 LANGUAGES "CXX")

configure_file("${CMAKE_SOURCE_DIR}/include/backend/version.cmake.in" "${CMAKE_SOURCE_DIR}/include/backend/version.hpp")

file(GLOB_RECURSE PUBLIC_SOURCE "src/*.cpp")
add_library(CorEngine STATIC ${PUBLIC_SOURCE})

file(GLOB_RECURSE PUBLIC_HEADERS "include/*.hpp")

target_include_directories(CorEngine PRIVATE 
"include"
"include/auxiliary"
"include/backend"
"include/engine"
"include/engine/audio"
"include/engine/core"
"include/engine/core/math"
"include/engine/core/windowing"
"include/engine/graphics"
"include/engine/input"
"include/engine/physics"
)

# Vulkan
find_package(Vulkan REQUIRED) 

target_link_libraries(
	CorEngine
	PUBLIC

	Vulkan::Vulkan
)
