
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>
#include "AppStructure.h"
#include "WindowManager.h"
#include "CorEngine.h"



VkInstance AppStructure::instance;
vec<std::shared_ptr<Window>> AppStructure::app_windows;
vec<VkPhysicalDevice> AppStructure::phys_devices;

AppStructure::AppStructure()
{
	delete this;
}

VkInstanceCreateInfo* AppStructure::createVkInfo(VkApplicationInfo app_info)
{
	VkInstanceCreateInfo createInfo{};

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &app_info;
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	return &createInfo;
}

VkApplicationInfo AppStructure::createVkAppInfo(const char* app_name, uint32_t app_version[4])
{
	VkApplicationInfo appInfo{};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = app_name;
	appInfo.applicationVersion = VK_MAKE_API_VERSION(app_version[0], app_version[1],
		app_version[2], app_version[3]);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	appInfo.pEngineName = "CorEngine";
	appInfo.engineVersion = VK_MAKE_API_VERSION(CORENGINE_VERSION_MAJOR,
		CORENGINE_VERSION_MINOR, CORENGINE_VERSION_PATCH,
		CORENGINE_VERSION_BUILD);

	return appInfo;
}

VkInstanceCreateInfo* AppStructure::redefineVkInfo(VkInstanceCreateInfo* c_info, VkApplicationInfo app_info)
{
	VkInstanceCreateInfo createInfo{};

	createInfo.sType = c_info->sType;
	createInfo.pApplicationInfo = &app_info;
	createInfo.enabledExtensionCount = c_info->enabledExtensionCount;
	createInfo.ppEnabledExtensionNames = c_info->ppEnabledExtensionNames;
	createInfo.enabledLayerCount = c_info->enabledLayerCount;
	createInfo.flags = c_info->flags;
	createInfo.pNext = c_info->pNext;
	createInfo.ppEnabledLayerNames = c_info->ppEnabledLayerNames;

	return &createInfo;
}

VkApplicationInfo AppStructure::redefineVkAppInfo(const VkApplicationInfo* a_info)
{
	VkApplicationInfo appInfo{};

	appInfo.sType = a_info->sType;
	appInfo.pNext = a_info->pNext;
	appInfo.pApplicationName = a_info->pApplicationName;
	appInfo.applicationVersion = a_info->applicationVersion;
	appInfo.apiVersion = a_info->apiVersion;
	appInfo.pEngineName = "CorEngine";
	appInfo.engineVersion = VK_MAKE_API_VERSION(CORENGINE_VERSION_MAJOR,
		CORENGINE_VERSION_MINOR, CORENGINE_VERSION_PATCH,
		CORENGINE_VERSION_BUILD);

	return appInfo;
}

void AppStructure::addWindow(Window* window)
{
	app_windows.push_back(std::shared_ptr<Window>(window));
	window->id = app_windows.size() - 1;
}

void AppStructure::remWindow(Window* window)
{
	app_windows[window->id].reset();
}

VkInstance* AppStructure::getInstance()
{
	return &instance;
}

unsigned int AppStructure::getWinQuantity()
{
	return AppStructure::app_windows.size();
}

void AppStructure::initVulkan(const char* app_name, uint32_t app_version[4])
{
	///
	VkResult res_0 = vkCreateInstance(createVkInfo(createVkAppInfo(app_name, app_version)),
		nullptr, &instance);
	if (res_0 != VK_SUCCESS)
		throw std::runtime_error("Failed to create Vulkan instance. VkResult code: " + res_0);
	///
	enumPhysDevices();
}

void AppStructure::initVulkan(VkInstanceCreateInfo* info, VkAllocationCallbacks* callbacks)
{
	///
	VkResult res_0 = vkCreateInstance(redefineVkInfo(info, redefineVkAppInfo(info->pApplicationInfo)),
		callbacks, &instance);
	if (res_0 != VK_SUCCESS)
		throw std::runtime_error("Failed to create Vulkan instance. VkResult code: " + res_0);
	///
	enumPhysDevices();
}

void AppStructure::enumPhysDevices()
{
	VkPhysicalDevice* start;
	uint32_t end;
	VkResult res_1 = vkEnumeratePhysicalDevices(instance, &end, start);
	if (res_1 != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to enumerate physical devices. VkResult code: " + res_1);
	}
	else
	{
		phys_devices.assign(start, start + end);
	}
}

void AppStructure::cleanup()
{
	vkDestroyInstance(instance, nullptr);

	for (size_t i = 0; i < app_windows.size(); i++)
	{
		app_windows[i].reset();
	}

	glfwTerminate();
}

