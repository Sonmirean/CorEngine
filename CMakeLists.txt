# Main cmake list for CorEngine.

cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20) 
endif()

project ("CorEngine" VERSION 0.1.4.2 LANGUAGES "CXX")

configure_file(${CMAKE_SOURCE_DIR}/backend/ver.cmake.in ${CMAKE_SOURCE_DIR}/backend/ver.hpp)

file(GLOB_RECURSE PUBLIC_SOURCE "src/*.cpp")
add_library(CorEngine STATIC ${PUBLIC_SOURCE})

# public headers path
target_include_directories(CorEngine PUBLIC 
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

# private headers path
target_include_directories(CorEngine PRIVATE 
"backend"
)






# Vulkan
find_package(Vulkan REQUIRED) 
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
