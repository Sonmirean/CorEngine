#pragma once

#define GLFW_INCLUDE_VULKAN

#include <memory>
#include <vector> 
#include <GLFW/glfw3.h>
#include "WindowManager.h"
#include "LoopManager.h"


template <typename T>
using vec = std::vector<T>;

// General structure of an application.
static class AppStructure
{
private:
	AppStructure();

	static VkInstance instance;
	static vec<std::shared_ptr<Window>> app_windows;
	static vec<VkPhysicalDevice> phys_devices;


	static VkInstanceCreateInfo* createVkInfo(VkApplicationInfo app_info);

	static VkApplicationInfo createVkAppInfo(const char* app_name, uint32_t app_version[4]);

	static VkInstanceCreateInfo* redefineVkInfo(VkInstanceCreateInfo* c_info, VkApplicationInfo app_info);

	static VkApplicationInfo redefineVkAppInfo(const VkApplicationInfo* a_info);


	/*
	Adds a Window object to application structure and defines its ID number.
	*/
	static void addWindow(Window* window);

	static void enumPhysDevices();

	friend class Window;
	friend class Heart;

public:


	/*
	Removes a Window object from application structure
	and deletes its instance.
	*/
	static void remWindow(Window* window);

	static VkInstance* getInstance();

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

	static void cleanup();
};
