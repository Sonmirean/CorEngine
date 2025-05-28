
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <thread>
#include <GLFW/glfw3.h>
#include "AppStructure.h"
#include "WindowManager.h"
#include "CorEngine.h"
#include "Types.h"
#include "Debug.h"

void PhysicalDevice::enumerateQueueFamilies()
{
	uint32_t queue_family_props_count;
	vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, nullptr);

	queue_families.reserve(queue_family_props_count);
	vec<VkQueueFamilyProperties> props = vec<VkQueueFamilyProperties>(queue_family_props_count);

	vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, props.data());

	for (size_t i = 0; i < queue_family_props_count; i++)
	{
		queue_families.emplace_back(this, props[i], i);
	}
} // void PhysicalDevice::enumerateQueueFamilies()


void PhysicalDevice::enumerateDeviceLayers()
{
	uint32_t layer_count;
	ensureVkSuccess(vkEnumerateDeviceLayerProperties(vk_handle, &layer_count, nullptr));
	if (layer_count != 0)
	{
		layer_props.reserve(layer_count);
		ensureVkSuccess(vkEnumerateDeviceLayerProperties(vk_handle, &layer_count, layer_props.data()));
	}
	else
	{
		throw std::exception("Error - No available layers found for current device.");
	}
} // PhysicalDevice::enumerateDeviceLayers()

VkPhysicalDeviceFeatures PhysicalDevice::getFeatures()
{
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(vk_handle, &features);
	return features;
} // VkPhysicalDeviceFeatures PhysicalDevice::getFeatures()

void PhysicalDevice::enumerateAll()
{
	uint32_t phys_devices_found;
	ensureVkSuccess(vkEnumeratePhysicalDevices(Application::instance, &phys_devices_found, nullptr));
	if (phys_devices_found > 0)
	{
		vec<VkPhysicalDevice> raw_devices(phys_devices_found);
		ensureVkSuccess(vkEnumeratePhysicalDevices(Application::instance, &phys_devices_found, raw_devices.data()));
		Application::phys_devices.reserve(phys_devices_found);
		for (unsigned int i = 0; i < phys_devices_found; i++)
		{
			Application::phys_devices.emplace_back(raw_devices[i]);
		}
	}
	else
	{
		throw std::exception("Error - No suitable graphical devices found.");
	}
} // void PhysicalDevice::enumerateAll()

PhysicalDevice::PhysicalDevice(VkPhysicalDevice vk_handle) : vk_handle(vk_handle)
{
	// Automatically enumerates all necessary things to work with that device.
	enumerateDeviceLayers();
	enumerateQueueFamilies();
} // PhysicalDevice::PhysicalDevice()

QueueFamily::QueueFamily(PhysicalDevice* p_parent, VkQueueFamilyProperties props, uint32_t index)
	: p_parent(p_parent), props(props), index(index) {} // QueueFamily::QueueFamily()

uint32_t QueueFamily::getIndex() 
{ 
	return index; 
} // QueueFamily::getIndex() 

Queue::Queue(VkQueue vk_handle, QueueFamily* const p_parent, VkDeviceQueueCreateInfo const info)
	: p_parent(p_parent) {} // Queue::Queue()

LogicalDevice::LogicalDevice(PhysicalDevice* p_parent, VkDeviceCreateInfo* p_info, VkAllocationCallbacks* p_allocator)
	: p_parent(p_parent)
{

	ensureVkSuccess(vkCreateDevice(p_parent->vk_handle, p_info, p_allocator, &vk_handle));

	//p_info.

	// TODO 

	/*for (size_t i = 0; i < queue_count; i++)
	{
		VkQueue queue;
		uint32_t queue_index;
		vkGetDeviceQueue(vk_handle, 0, &queue_index);

		desired_queue_families[i]->queues.emplace_back(, desired_queue_families[i], queue_infos[i]);
	}*/

	p_parent->logical_devices.push_back(std::move(*this));
} // LogicalDevice::LogicalDevice()

CommandPool::CommandPool(LogicalDevice* p_parent, QueueFamily* p_queue_family, 
	VkCommandPoolCreateFlagBits* p_flags_bitmask, VkAllocationCallbacks* p_allocator)
	: p_parent(p_parent)
{
	VkCommandPool pool;
	VkCommandPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = *p_flags_bitmask;
	pool_info.queueFamilyIndex = p_queue_family->index;

	ensureVkSuccess(vkCreateCommandPool(p_parent->vk_handle, &pool_info, p_allocator, &pool));
	vk_handle = pool;
	thread_id = std::this_thread::get_id();
	p_parent->command_pools.push_back(std::move(*this));
} // CommandPool::CommandPool()

namespace
{
	VkInstanceCreateInfo* createVkInfo(VkApplicationInfo app_info)
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
	VkApplicationInfo createVkAppInfo(const char* app_name, uint32_t app_version[4])
	{
		VkApplicationInfo appInfo{};

		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = app_name;
		appInfo.applicationVersion = VK_MAKE_API_VERSION(app_version[0], app_version[1],
			app_version[2], app_version[3]);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		appInfo.pEngineName = "CorEngine";
		appInfo.engineVersion = VK_MAKE_API_VERSION(CORENGINE_VERSION_MAJOR,
			CORENGINE_VERSION_MINOR, CORENGINE_VERSION_PATCH, CORENGINE_VERSION_BUILD);

		return appInfo;
	}
} // namespace

PhysicalDeviceGroup::PhysicalDeviceGroup(VkPhysicalDeviceGroupProperties props)
	: props(props) { }

void PhysicalDeviceGroup::enumerateAll()
{
	uint32_t group_count;
	vkEnumeratePhysicalDeviceGroups(Application::instance, &group_count, nullptr);
	vec<VkPhysicalDeviceGroupProperties> props(group_count);
	vkEnumeratePhysicalDeviceGroups(Application::instance, &group_count, props.data());

	for (size_t i = 0; i < group_count; i++)
	{
		PhysicalDeviceGroup group(props[i]);

		for (size_t i = 0; i < group.props.physicalDeviceCount; i++)
		{
			PhysicalDevice device(group.props.physicalDevices[i]);
			group.p_physical_devices.push_back(std::move(&device));
		}

		Application::phys_device_groups.push_back(std::move(group));
	}
}

size_t Application::getWinQuantity()
{
	return app_windows.size();
}

void Application::initVulkan(const char* app_name, uint32_t app_version[4])
{
	ensureVkSuccess(vkCreateInstance(createVkInfo(createVkAppInfo(app_name, app_version)), nullptr, &Application::instance));

	PhysicalDeviceGroup::enumerateAll();
	PhysicalDevice::enumerateAll();
}

void Application::finalCleanup()
{
	vkDestroyInstance(Application::instance, nullptr);

	for (size_t i = 0; i < app_windows.size(); i++)
	{
		app_windows[i].reset();
	}

	glfwTerminate();
}

