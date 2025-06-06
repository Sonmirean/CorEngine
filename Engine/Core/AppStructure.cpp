
#include <iostream>
#include <thread>
#include "AppStructure.h"
#include "WindowManager.h"
#include "CorEngine.h"
#include "Types.h"
#include "Debug.h"

QueueFamily::QueueFamily(PhysicalDevice* p_parent, VkQueueFamilyProperties* p_props, uint32_t index)
	: p_parent(p_parent), props(*p_props), index(index)
{

} // QueueFamily::QueueFamily()

void PhysicalDevice::enumerateQueueFamilies()
{
	uint32_t queue_family_props_count;
	vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, nullptr);
	vec<VkQueueFamilyProperties> props(queue_family_props_count);
	vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, props.data());

	for (size_t i = 0; i < queue_family_props_count; i++)
	{
		queue_families.push_back(std::move(QueueFamily(this, &props[i], i)));
	}
	queue_families.shrink_to_fit();
} // void PhysicalDevice::enumerateQueueFamilies()


void PhysicalDevice::enumerateDeviceLayers()
{
	uint32_t layer_count;
	ensureVkSuccess(vkEnumerateDeviceLayerProperties(vk_handle, &layer_count, nullptr));
	if (layer_count != 0)
	{
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

std::vector<LogicalDevice> PhysicalDevice::getLogicalDevices()
{
	return logical_devices;
}

std::vector<QueueFamily> PhysicalDevice::getQueueFamilies()
{
	return queue_families;
}

void PhysicalDevice::enumerateAll()
{
	uint32_t phys_devices_found;
	ensureVkSuccess(vkEnumeratePhysicalDevices(Application::instance, &phys_devices_found, nullptr));
	if (phys_devices_found > 0)
	{
		vec<VkPhysicalDevice> raw_devices(phys_devices_found);
		ensureVkSuccess(vkEnumeratePhysicalDevices(Application::instance, &phys_devices_found, raw_devices.data()));
		for (size_t i = 0; i < phys_devices_found; i++)
		{
			Application::phys_devices.push_back(std::move(PhysicalDevice(raw_devices[i])));
		}
		Application::phys_devices.shrink_to_fit();
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

uint32_t QueueFamily::getIndex() 
{ 
	return index; 
} // QueueFamily::getIndex() 

std::vector<Queue> QueueFamily::getQueues()
{
	return queues;
}

VkQueueFamilyProperties QueueFamily::getProps()
{
	return props;
}

Queue::Queue(QueueFamily* p_parent)
	: p_parent(p_parent) 
{
	p_parent->queues.push_back(std::move(*this));
} // Queue::Queue()

LogicalDevice::LogicalDevice(PhysicalDevice* p_parent, VkDeviceCreateInfo info, VkAllocationCallbacks allocator)
	: p_parent(p_parent)
{
	ensureVkSuccess(vkCreateDevice(p_parent->vk_handle, &info, &allocator, &vk_handle));

	p_parent->enumerateQueueFamilies();

	for (size_t i = 0; i < info.queueCreateInfoCount; i++)
	{
		Queue queue(&p_parent->queue_families[info.pQueueCreateInfos[i].queueFamilyIndex]);
	}
	for (size_t i = 0; i < p_parent->queue_families.size(); i++)
	{
		for (size_t j = 0; j < p_parent->queue_families[i].queues.size(); j++)
		{
			p_parent->queue_families[i].queues[j].index = j;
			VkQueue raw_queue;
			vkGetDeviceQueue(vk_handle, p_parent->queue_families[i].index, j, &raw_queue);
			p_parent->queue_families[i].queues[j].vk_handle = raw_queue;
		}
	}

	p_parent->logical_devices.push_back(std::move(*this));
} // LogicalDevice::LogicalDevice()

void CommandPool::trim()
{
	vkTrimCommandPool(p_parent->vk_handle, vk_handle, 0);
} // void CommandPool::trim()

void CommandPool::reset(VkCommandPoolResetFlags flags)
{
	ensureVkSuccess(vkResetCommandPool(p_parent->vk_handle, vk_handle, flags));
} // void CommandPool::reset()

void CommandPool::allocBuffers(std::vector<VkCommandBufferAllocateInfo> info)
{
	vec<VkCommandBuffer> raw_buffers(info.size());
	ensureVkSuccess(vkAllocateCommandBuffers(p_parent->vk_handle, info.data(), raw_buffers.data()));

	for (size_t i = 0; i < info.size(); i++)
	{
		command_buffers[i].vk_handle = raw_buffers[i];
	}
} // void CommandPool::allocBuffers()

CommandPool::~CommandPool()
{
	vkDestroyCommandPool(p_parent->vk_handle, vk_handle, nullptr);
} // CommandPool::~CommandPool()

QueueFamily CommandPool::getQueueFamily()
{
	return *p_queue_family;
}

std::thread::id CommandPool::getThreadID()
{
	return thread_id;
}

std::vector<CommandBuffer> CommandPool::getCommandBuffers()
{
	return command_buffers;
}

CommandPool::CommandPool(LogicalDevice* p_parent, QueueFamily* p_queue_family,
	VkCommandPoolCreateFlagBits* p_flags_bitmask, VkAllocationCallbacks* p_allocator)
	: p_parent(p_parent), p_queue_family(p_queue_family)
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
		uint32_t glfwExtensionCount;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		CORENGINE_DEBUG_PRINT(glfwExtensions[0])
		CORENGINE_DEBUG_PRINT(glfwExtensions[1])
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
} // anonymous namespace

PhysicalDeviceGroup::PhysicalDeviceGroup(VkPhysicalDeviceGroupProperties props)
	: p_props(&props) { }

void PhysicalDeviceGroup::enumerateAll()
{
	uint32_t group_count;
	vkEnumeratePhysicalDeviceGroups(Application::instance, &group_count, nullptr);
	vec<VkPhysicalDeviceGroupProperties> props(group_count);
	vkEnumeratePhysicalDeviceGroups(Application::instance, &group_count, props.data());

	for (size_t i = 0; i < group_count; i++)
	{
		PhysicalDeviceGroup group(props[i]);

		for (size_t i = 0; i < group.p_props->physicalDeviceCount; i++)
		{
			PhysicalDevice device(group.p_props->physicalDevices[i]);
			group.p_physical_devices.push_back(&device);
		}
		group.p_physical_devices.shrink_to_fit();
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
		app_windows[i]->~Window();
	}

	glfwTerminate();
}

