#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define  GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN

#include <filesystem>
#include <GLFW/glfw3.h>]
#include <iostream>

extern void ensureVkSuccess(VkResult res);

#ifdef CORENGINE_DEBUG_PRINT_ENABLED

#ifdef CORENGINE_DEBUG_PRINT_VERBOSE
#define CORENGINE_DEBUG_PRINT(var) \
	std::cout  \
	<< "\033[36m[CorEngine debug] - " \
	<< "\033[0m" << std::filesystem::path(__FILE__) << "\033[36m/" \
	<< "\033[0m" << __LINE__ << "\033[36m/" \
	<< "\033[33m" << #var << "\033[0m = \033[32m" << var << "\033[0m" \
	<< std::endl;
#else
#define CORENGINE_DEBUG_PRINT(var) \
	std::cout  \
	<< "\033[36m[CorEngine debug] - " \
	<< "\033[0m" << std::filesystem::path(__FILE__).filename() << "\033[36m/" \
	<< "\033[0m" << __LINE__ << "\033[36m/" \
	<< "\033[33m" << #var << "\033[0m = \033[32m" << var << "\033[0m" \
	<< std::endl;
#endif //CORENGINE_DEBUG_PRINT_VERBOSE

#else
#define CORENGINE_DEBUG_PRINT(var)

#endif //CORENGINE_DEBUG_PRINT_ENABLED
