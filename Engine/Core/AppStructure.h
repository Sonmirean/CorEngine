#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "LoopManager.h"
#include "Types.h"

namespace AppStructure
{
	namespace
	{
		VkInstance					 instance;

		vec<sptr<Window>>			 app_windows;
		vec<VkPhysicalDevice>		 phys_devices;
		VkAllocationCallbacks		 alloc_callbacks;

		VkInstanceCreateInfo* createVkInfo(VkApplicationInfo app_info);
		VkApplicationInfo createVkAppInfo(const char* app_name, uint32_t app_version[4]);
		VkInstanceCreateInfo* redefineVkInfo(VkInstanceCreateInfo* c_info, VkApplicationInfo app_info);
		VkApplicationInfo redefineVkAppInfo(const VkApplicationInfo* a_info);

		void enumPhysDevices();
	}

	static class ExternalClassAccess
	{
	public:
		static void addWindow(Window* window);
	private:
		static void _addWindow(Window* window);
	};



	static unsigned int getWinQuantity();

	/*
	Creates Vulkan instance and initializes essentials.

	Enumerates physical devices attached to this device.
	*/
	static void initVulkan(const char* app_name, uint32_t app_version[4]);

	/*
	Initializes Vulkan using user-defined methods.

	Note that the engine information would be whatever
	replaced with those of CorEngine.
	*/
	static void initVulkan(VkInstanceCreateInfo* info, VkAllocationCallbacks* callbacks);

	static void finalCleanup();

}
