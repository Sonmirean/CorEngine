#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

template <typename T>
using vec = std::vector<T>;
template <typename T>
using sptr = std::shared_ptr<T>;
