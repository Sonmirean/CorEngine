#pragma once

#define GLFW_INCLUDE_VULKAN
#include "auxiliary/data_types.hpp"

/**
* Loads 3-dimensional model vertex data from .obj file.
* Note that non-triangle polygons are NOT supported.
* 
* @param const char* file_path - Path of .obj file to load.
* @returns A Model object.
*/
Dim3::Model_3D loadModelOBJ(const char* file_path);