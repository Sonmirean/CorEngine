#pragma once

#define GLFW_INCLUDE_VULKAN

#include <thread>
#include <GLFW/glfw3.h>
#include "LoopManager.h"
#include "Types.h"
#include "Debug.h"


struct CommandBuffer;
struct Queue;
struct QueueFamily;


/**
* Physical device represents a connection to a graphical processing unit
* or another device that is recognized as suitable for rendering by Vulkan.
* One must not try to create any objects with this struct manually. To query
* the list of available physical devices, call PhysicalDevice::enumerateAll().
*/
struct PhysicalDevice
{
public:
	// Gets Vulkan features of that device.
	VkPhysicalDeviceFeatures getFeatures();

	// Enumerates all attached graphical processing units, or other
	// devices that are recognized as suitable for rendering by Vulkan.
	static void enumerateAll();

private:
	friend struct LogicalDevice;
	friend struct PhysicalDeviceGroup;
	// Constructor for internal use.
	PhysicalDevice(VkPhysicalDevice vk_handle);

	// Logical devices created with this physical device as a base.
	vec<LogicalDevice> logical_devices{};
	// Queue families available on a physical device
	vec<QueueFamily> queue_families{};
	// Device layers available for current physical device.
	vec<VkLayerProperties> layer_props{};

	// Enumerates all available layers for this device.
	void enumerateDeviceLayers();
	// Enumerates all the queue families available on this device.
	void enumerateQueueFamilies();

	// Vulkan handle of this wrap.
	VkPhysicalDevice vk_handle;

}; // struct PhysicalDevice


/**
* Command pool is a pool of memory allocated for command buffers.
* A single command pool must NOT be used concurrently in multiple threads.
*/
struct CommandPool
{
public:
	// Pointer to a parent struct.
	LogicalDevice* p_parent;
	// Vulkan handle of this wrap.
	VkCommandPool vk_handle;

	/**
	* Command pool is a pool of memory allocated for command buffers.
	* A single command pool must NOT be used concurrently in multiple threads.
	* Must be created from a thread that is desired to be its owner.
	*
	* TODO - add param documentation.
	*/
	CommandPool(LogicalDevice* p_parent, QueueFamily* p_queue_family, VkCommandPoolCreateFlagBits* p_flags_bitmask,
		VkAllocationCallbacks* p_allocator);

private:

	// Pointer to a thread which owns this command pool.
	std::thread::id thread_id;
	// Command buffers allocated in this command pool.
	vec<CommandBuffer> cmd_bufs{};


}; //struct CommandPool

/**
* Command buffer is an object used to record and submit Vulkan commands.
*/
struct CommandBuffer
{
public:
	// TODO - something
private:
	// Pointer to a parent struct.
	CommandPool* p_parent;
	// Vulkan handle of this wrap.
	VkCommandBuffer vk_handle;
};

/**
* Queue of commands submitted to a device.
*/
struct Queue
{
public:

private:

	// Pointer to a parent struct.
	QueueFamily* p_parent;
	// Vulkan handle of this wrap.
	VkQueue vk_handle;

	Queue(VkQueue vk_handle, QueueFamily* const p_parent, VkDeviceQueueCreateInfo const info);

}; // struct Queue

/**
* Queue family is an union of queues with identical features.
*/
struct QueueFamily
{
public:
	// Pointer to a parent struct.
	PhysicalDevice* p_parent;

	// Gets the index of this queue family.
	// Index is necessary when creating anything with a queue family.
	uint32_t getIndex();

private:
	friend struct PhysicalDevice;
	friend struct LogicalDevice;
	friend struct CommandPool;

	// Queues belonging to this family.
	vec<Queue> queues{};

	// Properties of this queue family
	VkQueueFamilyProperties props;
	// Index of this queue family. 
	uint32_t index;


	QueueFamily(PhysicalDevice* p_parent, VkQueueFamilyProperties props, uint32_t index);

}; // struct QueueFamily

/**
* Logical device represents a logical connection to a physical device.
* It's one of the primary objects to interact with Vulkan implementation.
*/
struct LogicalDevice
{
public:
	/**
	* Logical device represents a logical connection to physical device.
	* It's one of the primary objects to interact with Vulkan implementation.
	*
	* @param PhysicalDevice* p_parent - A device to which create a connection.
	* @param VkDeviceCreateInfo info - General info about device creation.
	* @param VkAllocationCallbacks allocator - Allocator.
	*
	*/
	LogicalDevice(PhysicalDevice* p_parent, VkDeviceCreateInfo* p_info, VkAllocationCallbacks* p_allocator);

private:
	friend struct CommandPool;
	// Command pools created with usage of this device.
	vec<CommandPool> command_pools{};

	// Vulkan handle of this wrap.
	VkDevice vk_handle;
	// Pointer to a parent struct.
	PhysicalDevice* p_parent;

}; // struct LogicalDevice

/**
* This struct represents a group of physical devices of the same vendor
* which can be represented as a single logical device to combine their memory.
* 
* TODO - make adecuate enumeration method, i.e. make it enumerate devices as well.
*/
struct PhysicalDeviceGroup
{
	// Enumerates all available groups of devices.
	static void enumerateAll();

private:
	// List of a pointers to each physical device of the group.
	vec<PhysicalDevice*> p_physical_devices;
	// Properties of this device group.
	VkPhysicalDeviceGroupProperties props;
	
	PhysicalDeviceGroup(VkPhysicalDeviceGroupProperties props);
};

/**
* This static struct represents an application built with this library.
* Creation of any objects with it, as well as inheritance of it
* is considered as an undefined behaviour.
*/
struct Application
{
	/**
	* General technical parameters submitted to an
	* application right in the moment of creation.
	*/
	struct Parameters
	{
		
	};
	// Parameters of the application
	static const Parameters param;

	// Vulkan instance - basement of everything in Vulkan.
	static inline VkInstance instance{};
	// Physical devices found by current Vulkan instance.
	static inline vec<PhysicalDevice> phys_devices{};
	// List of physical device groups.
	static inline vec<PhysicalDeviceGroup> phys_device_groups{};

	// Windows of this application.
	static inline vec<sptr<Window>> app_windows{};

	// Returns quantity of app windows.
	static size_t getWinQuantity();

	// Creates Vulkan instance and initializes essentials.
	static void initVulkan(const char* app_name, uint32_t app_version[4]);

	static void finalCleanup();

	Application() = delete;

}; // struct Application
