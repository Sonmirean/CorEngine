
#include <iostream>

#include "CorE/engine/core_manager.hpp"
#include "CorE/engine/windowing/window_manager.hpp"

#include "CorE/auxiliary/short_type.hpp"
#include "CorE/auxiliary/debug.hpp"

CorE::QueueFamily::QueueFamily(PhysicalDevice* p_parent, VkQueueFamilyProperties* p_props, uint32_t index)
	: p_parent(p_parent), props(*p_props), index(index)
{

} // QueueFamily::QueueFamily()

void CorE::PhysicalDevice::enumerateQueueFamilies()
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

void CorE::PhysicalDevice::enumerateDeviceLayers()
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

VkPhysicalDeviceFeatures CorE::PhysicalDevice::getFeatures()
{
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(vk_handle, &features);
	return features;
} // VkPhysicalDeviceFeatures PhysicalDevice::getFeatures()

std::vector<CorE::LogicalDevice> CorE::PhysicalDevice::getLogicalDevices()
{
	return logical_devices;
} // std::vector<LogicalDevice> PhysicalDevice::getLogicalDevices()

std::vector<CorE::QueueFamily> CorE::PhysicalDevice::getQueueFamilies()
{
	return queue_families;
} // std::vector<QueueFamily> PhysicalDevice::getQueueFamilies()

void CorE::PhysicalDevice::enumerateAll()
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
			//Application::phys_devices.back().enumerateDisplays();
		}
		Application::phys_devices.shrink_to_fit();
	}
	else
	{
		throw std::exception("Error - No suitable graphical devices found.");
	}
} // void PhysicalDevice::enumerateAll()

CorE::PhysicalDevice::PhysicalDevice(VkPhysicalDevice vk_handle) : vk_handle(vk_handle)
{
	// Automatically enumerates all necessary things to work with that device.
	enumerateDeviceLayers();
	enumerateQueueFamilies();
} // PhysicalDevice::PhysicalDevice()

uint32_t CorE::QueueFamily::getIndex()
{ 
	return index; 
} // QueueFamily::getIndex() 

std::vector<CorE::Queue> CorE::QueueFamily::getQueues()
{
	return queues;
} // std::vector<Queue> QueueFamily::getQueues()

VkQueueFamilyProperties CorE::QueueFamily::getProps()
{
	return props;
} // VkQueueFamilyProperties QueueFamily::getProps()

CorE::Queue::Queue(QueueFamily* p_parent)
	: p_parent(p_parent) 
{
	p_parent->queues.push_back(std::move(*this));
} // Queue::Queue()

CorE::LogicalDevice::LogicalDevice(PhysicalDevice* p_parent, VkDeviceCreateInfo info, VkAllocationCallbacks allocator)
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

void CorE::CommandPool::trim()
{
	vkTrimCommandPool(p_parent->vk_handle, vk_handle, 0);
} // void CommandPool::trim()

void CorE::CommandPool::reset(VkCommandPoolResetFlags flags)
{
	ensureVkSuccess(vkResetCommandPool(p_parent->vk_handle, vk_handle, flags));
} // void CommandPool::reset()

void CorE::CommandPool::allocBuffers(std::vector<VkCommandBufferLevel> levels, std::vector<uint32_t> quantities)
{
	vec<VkCommandBufferAllocateInfo> infos(levels.size());
	bool single_per_level;
	quantities.empty() ? single_per_level = true : single_per_level = false;
	for (size_t i = 0; i < levels.size(); i++)
	{
		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = vk_handle;
		info.level = levels[i];
		info.commandBufferCount = single_per_level ? 1 : quantities[i];
		info.commandBufferCount = quantities[i];
		infos.push_back(info);
	}
	vec<VkCommandBuffer> raw_buffers(levels.size());
	ensureVkSuccess(vkAllocateCommandBuffers(p_parent->vk_handle, infos.data(), raw_buffers.data()));

	for (size_t i = 0; i < raw_buffers.size(); i++)
	{
		CommandBuffer buffer(raw_buffers[i], this, command_buffers.size() + 1);

		command_buffers.push_back(buffer);
	}
} // void CommandPool::allocBuffers()

void CorE::CommandPool::freeBuffers(std::vector<uint32_t> buffer_indices)
{
	for (size_t i = 0; i < buffer_indices.size(); i++)
	{
		command_buffers.erase(command_buffers.begin() + buffer_indices[i]);
	}
} // void CommandPool::freeBuffers()

CorE::CommandPool::~CommandPool()
{
	vkDestroyCommandPool(p_parent->vk_handle, vk_handle, nullptr);
} // CommandPool::~CommandPool()

CorE::QueueFamily CorE::CommandPool::getQueueFamily()
{
	return *p_queue_family;
} // QueueFamily CommandPool::getQueueFamily()

std::thread::id CorE::CommandPool::getThreadID()
{
	return thread_id;
} // std::thread::id CommandPool::getThreadID()

std::vector<CorE::CommandBuffer> CorE::CommandPool::getCommandBuffers()
{
	return command_buffers;
} // std::vector<CommandBuffer> CommandPool::getCommandBuffers()

CorE::CommandPool::CommandPool(LogicalDevice* p_parent, QueueFamily* p_queue_family,
	VkCommandPoolCreateFlagBits flags_bitmask, const VkAllocationCallbacks* p_allocator)
	: p_parent(p_parent), p_queue_family(p_queue_family)
{
	VkCommandPool pool;
	VkCommandPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = flags_bitmask;
	pool_info.queueFamilyIndex = p_queue_family->index;

	ensureVkSuccess(vkCreateCommandPool(p_parent->vk_handle, &pool_info, p_allocator, &pool));
	vk_handle = pool;
	thread_id = std::this_thread::get_id();
	p_parent->command_pools.push_back(std::move(*this));
} // CommandPool::CommandPool()

void CorE::CommandBuffer::begin(VkCommandBufferUsageFlags flags, VkCommandBufferInheritanceInfo* p_inherit_info)
{
	const VkCommandBufferBeginInfo info
	{
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		nullptr,
		flags,
		p_inherit_info
	};

	ensureVkSuccess(vkBeginCommandBuffer(vk_handle, &info));
} // void CommandBuffer::begin()

void CorE::CommandBuffer::end()
{
	ensureVkSuccess(vkEndCommandBuffer(vk_handle));
} // void CommandBuffer::end()

void CorE::CommandBuffer::chain(std::vector<CorE::CommandBuffer*> buffers)
{
	for (size_t i = 0; i < buffers.size(); i++)
	{
		vkCmdExecuteCommands(vk_handle, 1, &buffers[i]->vk_handle);
		p_next = buffers[i];
		buffers[i]->p_prev = this;
	}
} // void CommandBuffer::chain()

CorE::CommandBuffer::CommandBuffer(VkCommandBuffer vk_handle, CommandPool* p_parent, uint32_t index)
	: vk_handle(vk_handle), p_parent(p_parent), index(index)
{

} // CommandBuffer::CommandBuffer()

namespace
{
	VkInstanceCreateInfo* createVkInstanceInfo(VkApplicationInfo app_info)
	{
		VkInstanceCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &app_info;
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = /*glfwGetRequiredInstanceExtensions(&glfwExtensionCount)*/nullptr;
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

CorE::PhysicalDeviceGroup::PhysicalDeviceGroup(VkPhysicalDeviceGroupProperties props)
	: p_props(&props) { } // PhysicalDeviceGroup::PhysicalDeviceGroup()

void CorE::PhysicalDeviceGroup::enumerateAll()
{
	uint32_t group_count;
	ensureVkSuccess(vkEnumeratePhysicalDeviceGroups(Application::instance, &group_count, nullptr));
	vec<VkPhysicalDeviceGroupProperties> props(group_count);
	ensureVkSuccess(vkEnumeratePhysicalDeviceGroups(Application::instance, &group_count, props.data()));

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
} // void PhysicalDeviceGroup::enumerateAll()

void CorE::PhysicalDevice::enumerateDisplays()
{
	// only one display for now.
	uint32_t count;
	ensureVkSuccess(vkGetPhysicalDeviceDisplayPropertiesKHR(vk_handle, &count, nullptr));
	CORENGINE_DEBUG_PRINT(count)
	vec<VkDisplayPropertiesKHR> raw_displays(count);
	ensureVkSuccess(vkGetPhysicalDeviceDisplayPropertiesKHR(vk_handle, &count, raw_displays.data()));
	for (size_t i = 0; i < count; i++)
	{
		displays.push_back(std::move(Display(raw_displays[i], this)));
		std::cout << raw_displays[i].physicalResolution.width << " " << raw_displays[i].physicalResolution.height << std::endl;
	}
} // PhysicalDevice::enumerateDisplays()

size_t CorE::Application::getWinQuantity()
{
	return app_windows.size();
} // size_t Application::getWinQuantity()

void CorE::Application::initVulkan(const char* app_name, uint32_t app_version[4])
{
	ensureVkSuccess(vkCreateInstance(createVkInstanceInfo(createVkAppInfo(app_name, app_version)), nullptr, &Application::instance));

	PhysicalDeviceGroup::enumerateAll();
	PhysicalDevice::enumerateAll();
} // void Application::initVulkan()

void CorE::Application::finalCleanup()
{
	vkDestroyInstance(Application::instance, nullptr);
	//glfwTerminate();
} // void Application::finalCleanup()

CorE::Display::Display(VkDisplayPropertiesKHR props, PhysicalDevice* p_phys_device)
	: props(props), p_parent(p_phys_device) {} // Display::Display()

VkSemaphoreType CorE::Queue::Semaphore::getType()
{
	return type;
}
