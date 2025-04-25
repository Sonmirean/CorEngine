#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>

void checkVkSuccess(VkResult res)
{
	if (res != VK_SUCCESS)
		throw std::runtime_error("Failed to create Vulkan instance. VkResult code: " + res);
}