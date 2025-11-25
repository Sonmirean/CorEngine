# Main cmake list for CorEngine.

cmake_minimum_required (VERSION 4.0)
set (CMAKE_CXX_STANDARD 20)
project("CorEngine" VERSION 0.1.4.3 LANGUAGES "CXX")

configure_file("${CMAKE_SOURCE_DIR}/include/CorE/version.cmake.in" "${CMAKE_SOURCE_DIR}/include/CorE/version.hpp")
file(GLOB_RECURSE PUBLIC_SOURCE "src/*.cpp") # dubious lines of dubious code
file(GLOB_RECURSE PUBLIC_HEADER "include/*.hpp")
add_library(CorEngine STATIC ${PUBLIC_SOURCE} "dependencies/slang-2025.19.1-windows-x86_64/include/slang-com-helper.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-com-ptr.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-cpp-host-prelude.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-cpp-prelude.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-cpp-scalar-intrinsics.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-cpp-types-core.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-cpp-types.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-cuda-prelude.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-deprecated.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-gfx.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-hlsl-prelude.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-image-format-defs.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-llvm.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-tag-version.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang-torch-prelude.h" "dependencies/slang-2025.19.1-windows-x86_64/include/slang.h")
target_include_directories(CorEngine PRIVATE "include")
target_include_directories(
	CorEngine 
	PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

# build for weendows
add_compile_definitions(CORENGINE_BUILDFOR_WINDOWS)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/slang-2025.19.1-windows-x86_64/cmake")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

target_link_directories(
	CorEngine 
	PRIVATE 
	"dependencies/slang-2025.18.1-windows-aarch64/lib" # wtf aarch64??? TODO
)

target_link_libraries(
	CorEngine
	PUBLIC
	# (Vulkan)
	Vulkan::Vulkan
)# vulkan vulkan

find_package(Vulkan REQUIRED)
find_package(slang REQUIRED)

# TODO - Add compilation of all /shaders upon build, and move them to install dir upon install.

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
	DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake/CorEngine"
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	"CorEngineConfig.cmake.in" 
	"${CMAKE_CURRENT_BINARY_DIR}/CorEngineConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake/CorEngine/"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/CorEngineConfigVersion.cmake"
  VERSION 0.1.3
  COMPATIBILITY SameMajorVersion
)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/CorEngineConfig.cmake"
		  "${CMAKE_CURRENT_BINARY_DIR}/CorEngineConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake/CorEngine"
)
