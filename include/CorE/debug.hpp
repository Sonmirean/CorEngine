#pragma once

#include <filesystem>
#include <iostream>

#include <vulkan/vk_enum_string_helper.h>

extern void ensureVkSuccess(VkResult res, std::string message);

#ifdef CORENGINE_DEBUG_PRINT_ENABLED

#define CORENGINE_DEBUG_PRINT(var) \
	std::cout  \
	<< "\033[36m[CorEngine debug] - " \
	<< "\033[0m" << std::filesystem::path(__FILE__).filename() << "\033[36m/" \
	<< "\033[0m" << __LINE__ << "\033[36m/" \
	<< "\033[33m" << #var << "\033[0m = \033[32m" << var << "\033[0m" \
	<< std::endl;

#else
#define CORENGINE_DEBUG_PRINT(var)

#endif //CORENGINE_DEBUG_PRINT_ENABLED
