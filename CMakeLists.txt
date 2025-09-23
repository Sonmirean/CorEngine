# Main cmake list for CorEngine.

cmake_minimum_required (VERSION 4.0)
project("CorEngine" VERSION 0.1.4.3 LANGUAGES "CXX")

configure_file("${CMAKE_SOURCE_DIR}/include/CorE/auxiliary/version.cmake.in" "${CMAKE_SOURCE_DIR}/include/CorE/auxiliary/version.hpp")
file(GLOB_RECURSE PUBLIC_SOURCE "src/*.cpp")
file(GLOB_RECURSE PUBLIC_HEADER "include/*.hpp")
add_library(CorEngine STATIC ${PUBLIC_SOURCE})
target_include_directories(CorEngine PRIVATE "include")
target_include_directories(
	CorEngine PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>$<INSTALL_INTERFACE:include>
)

# Vulkan
find_package(Vulkan REQUIRED) 

target_link_libraries(
	CorEngine
	PUBLIC

	Vulkan::Vulkan
)

install(
	TARGETS CorEngine
	EXPORT CorEngineTargets
	ARCHIVE DESTINATION "lib"
)
install(
	DIRECTORY "include/" 
	DESTINATION include
)
install(
	EXPORT CorEngineTargets
	FILE CorEngineTargets.cmake
	NAMESPACE CorE::
	DESTINATION "lib/cmake/CorEngine"
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	"CorEngineConfig.cmake.in" 
	"${CMAKE_CURRENT_BINARY_DIR}/CorEngineConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/CorEngine/"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/CorEngineConfigVersion.cmake"
  VERSION 0.1.3
  COMPATIBILITY SameMajorVersion
)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/CorEngineConfig.cmake"
		  "${CMAKE_CURRENT_BINARY_DIR}/CorEngineConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/CorEngine"
)


## When trying to find this project's package, CMake fails.
## TODO - Fix the install to make project findable