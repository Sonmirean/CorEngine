#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "DataTypes/Models.h"

/**
* Loads 3-dimensional model vertex data from .obj file.
* Note that non-triangle polygons are NOT supported.
* 
* @param const char* file_path - Path of .obj file to load.
* @returns A Model object.
*/
Model loadModel(const char* file_path);