#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "LoopManager.h"
#include "Types.h"
#include "Debug.h"

namespace AppStructure
{

	static class ExternalAccess
	{
	private:
		friend class Window;
		static void addWindow(Window* window);
	};

	static unsigned int getWinQuantity();

	/*
	Creates Vulkan instance and initializes essentials.

	Enumerates physical devices attached to this device.
	*/
	void initVulkan(const char* app_name, uint32_t app_version[4]);
	/*
	Initializes Vulkan using user-defined methods.

	Note that the engine information would be whatever
	replaced with those of CorEngine.
	*/
	void initVulkan(VkInstanceCreateInfo* info, VkAllocationCallbacks* callbacks);



	static void finalCleanup();

}
