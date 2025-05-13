
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <thread>
#include <GLFW/glfw3.h>
#include "AppStructure.h"
#include "WindowManager.h"
#include "CorEngine.h"
#include "Types.h"
#include "Debug.h"

namespace AppStructure
{

	PhysicalDevice::LogicalDevice::LogicalDevice(PhysicalDevice* p_parent, uint32_t queue_count, std::vector<VkDeviceQueueCreateInfo> const queue_infos,
		std::vector<const char*> const enabled_extension_names, VkPhysicalDeviceFeatures* const p_features)
		// TODO - add custom allocator support
	{
		p_parent = p_parent;
		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.queueCreateInfoCount = queue_count;
		info.pQueueCreateInfos = queue_infos.data();
		info.enabledExtensionCount = enabled_extension_names.size();
		info.ppEnabledExtensionNames = enabled_extension_names.data();
		info.pEnabledFeatures = p_features;

		_ensureVkSuccess(vkCreateDevice(p_parent->vk_handle, &info, nullptr, &vk_handle));
		p_parent->logical_devices.push_back(*this);
	} // PhysicalDevice::LogicalDevice::LogicalDevice()

	PhysicalDevice::LogicalDevice::CommandPool::CommandPool(LogicalDevice* p_parent, QueueFamily* p_queue_family, VkCommandPoolCreateFlagBits* p_flags_bitmask,
		VkAllocationCallbacks* p_allocator)
		: p_parent(p_parent)
	{
		VkCommandPool pool;
		VkCommandPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.flags = *p_flags_bitmask;
		pool_info.queueFamilyIndex = p_queue_family->index;

		_ensureVkSuccess(vkCreateCommandPool(p_parent->vk_handle, &pool_info, p_allocator, &pool));
		vk_handle = pool;
		thread_id = std::this_thread::get_id();
		p_parent->command_pools.push_back(*this);
	} // PhysicalDevice::LogicalDevice::CommandPool::CommandPool()


	PhysicalDevice::QueueFamily::Queue::Queue(LogicalDevice* p_parent, VkDeviceQueueCreateFlags flags, 
		uint32_t queue_family_index, const float* p_priorities, uint32_t count)
		: p_parent(p_parent)
	{
		if (queue_family_index <= p_parent->p_parent->queue_families.size())
		{
			VkDeviceQueueCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			info.flags = flags;
			info.queueCount = count;
			info.queueFamilyIndex = queue_family_index;
			info.pQueuePriorities = p_priorities;
		}
		else
		{
			throw std::exception("Error - Desired queue family is absent on current GPU.");
		}
	}; // PhysicalDevice::Queue::Queue()


	void PhysicalDevice::enumerateDeviceLayers()
	{
		uint32_t layer_count;
		_ensureVkSuccess(vkEnumerateDeviceLayerProperties(vk_handle, &layer_count, nullptr));
		if (layer_count != 0)
		{
			layer_props.reserve(layer_count);
			_ensureVkSuccess(vkEnumerateDeviceLayerProperties(vk_handle, &layer_count, layer_props.data()));
			CORENGINE_DEBUG_PRINT(layer_count)
		}
		else
		{
			throw std::exception("Error - No available layers found for current device.");
		}
	} // PhysicalDevice::enumerateDeviceLayers()

	void PhysicalDevice::enumerateQueueFamilyProps()
	{
		uint32_t queue_family_props_count;
		vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, nullptr);

		queue_families = vec<VkQueueFamilyProperties>(queue_family_props_count);
		vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, queue_families.data());

		CORENGINE_DEBUG_PRINT(queue_family_props_count);
	} // PhysicalDevice::enumerateQueueFamilyProps()

	VkPhysicalDeviceFeatures PhysicalDevice::getFeatures()
	{
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(vk_handle, &features);
		return features;
	}

	// Enumerates all attached graphical processing units, or other
	// devices that are recognized as suitable for rendering by Vulkan.
	void PhysicalDevice::enumerateAttached()
	{
		uint32_t phys_devices_found;
		_ensureVkSuccess(vkEnumeratePhysicalDevices(instance, &phys_devices_found, nullptr));
		if (phys_devices_found > 0)
		{
			vec<VkPhysicalDevice> raw_devices(phys_devices_found);
			_ensureVkSuccess(vkEnumeratePhysicalDevices(instance, &phys_devices_found, raw_devices.data()));
			phys_devices.reserve(phys_devices_found);
			for (unsigned int i = 0; i < phys_devices_found; i++)
			{
				PhysicalDevice phys_device = PhysicalDevice(raw_devices[i]);
				phys_devices.push_back(phys_device);
			}
			CORENGINE_DEBUG_PRINT(phys_devices_found)
		}
		else
		{
			throw std::exception("Error - No attached suitable physical devices found.");
		}
	} // PhysicalDevice::enumerateAttached()

	PhysicalDevice::PhysicalDevice(VkPhysicalDevice vk_handle) : vk_handle(vk_handle)
	{
		enumerateQueueFamilyProps();
	}

	void enumeratePhysDeviceGroupProps()
	{
		uint32_t phys_device_group_props_count;
		_ensureVkSuccess(vkEnumeratePhysicalDeviceGroups(AppStructure::instance, &phys_device_group_props_count, nullptr));
		if (phys_device_group_props_count != 0)
		{
			AppStructure::phys_device_groups.reserve(phys_device_group_props_count);
			_ensureVkSuccess(vkEnumeratePhysicalDeviceGroups(AppStructure::instance, &phys_device_group_props_count, AppStructure::phys_device_groups.data()));

			CORENGINE_DEBUG_PRINT(phys_device_group_props_count);
		}
	} // enumeratePhysDeviceGroupProps()

	namespace
	{
		vec<sptr<Window>> app_windows;

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
				CORENGINE_VERSION_MINOR, CORENGINE_VERSION_PATCH,
				CORENGINE_VERSION_BUILD);

			return appInfo;
		}

	}// namespace

	void AppStructure::ExternalAccess::addWindow(Window* window)
	{
		app_windows.push_back(std::shared_ptr<Window>(window));
		window->id = app_windows.size() - 1;
	}

	unsigned int getWinQuantity()
	{
		return app_windows.size();
	}

	void initVulkan(const char* app_name, uint32_t app_version[4])
	{
		_ensureVkSuccess(vkCreateInstance(createVkInfo(createVkAppInfo(app_name, app_version)), nullptr, &AppStructure::instance));

		enumeratePhysDeviceGroupProps();
		AppStructure::PhysicalDevice::enumerateAttached();
	}

	void finalCleanup()
	{
		vkDestroyInstance(AppStructure::instance, nullptr);

		for (size_t i = 0; i < app_windows.size(); i++)
		{
			app_windows[i].reset();
		}

		glfwTerminate();
	}
}