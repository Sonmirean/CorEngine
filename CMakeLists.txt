# Main cmake list for CorEngine.

cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CorEngine" VERSION 0.1.3  )
set(PROJECT_VERSION_BUILD          1)

configure_file(
    ${CMAKE_SOURCE_DIR}/CorEngine.in 
    ${CMAKE_SOURCE_DIR}/CorEngine.hpp
)

set(CPM_DOWNLOAD_VERSION 0.27.2) 
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
message(STATUS "${CPM_DOWNLOAD_LOCATION}")
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

add_executable (${PROJECT_NAME} "CorEngine.cpp" "Engine/Core/AppStructure.cpp" "Engine/Core/AppStructure.h" "CorEngine.h"  "Engine/Graphics/RenderManager.cpp" "Engine/Input/MouseInput.cpp" "Engine/Input/KeyboardInput.cpp" "Engine/Core/WindowManager.cpp" "Engine/Utils/DimensionUtils.h" "Engine/Core/LoopManager.cpp" "Engine/Core/LoopManager.h" "Engine/Graphics/RenderManager.h" "Engine/Core/Entity/Entity.h" "Engine/Core/Entity/Model.h" "Engine/Core/Entity/Texture.h" "Engine/Core/Entity/Texture.cpp" "Engine/Utils/Types.h")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Core")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Utils")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20) 
endif()

# glfw
CPMAddPackage(
  NAME glfw
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.3.8
  OPTIONS
    "GLFW_BUILD_DOCS OFF"
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_EXAMPLES OFF"
)

# glm
CPMAddPackage(
  NAME glm
  GITHUB_REPOSITORY g-truc/glm
  GIT_TAG 0.9.9.8
)

# linking
target_link_libraries(${PROJECT_NAME} glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})

# vulkan
find_package(Vulkan REQUIRED) 
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

