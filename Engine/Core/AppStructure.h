#pragma once

#define GLFW_INCLUDE_VULKAN

#include <thread>
#include <GLFW/glfw3.h>
#include "LoopManager.h"
#include "Types.h"
#include "Debug.h"

namespace AppStructure
{

	// Vulkan instance - root of the hierarchy tree, and the basement of everything in Vulkan.
	VkInstance instance;

	/**
	Physical device represents a connection to a graphical processing unit
	or another device that is recognized as suitable for rendering by Vulkan.
	One must not try to create any objects with this struct manually. To query
	the list of available physical devices, call PhysicalDevice::enumerateAttached().
	*/
	struct PhysicalDevice
	{
	public:
		// Vulkan handle of this wrap.
		VkPhysicalDevice vk_handle;

		// Queue family is an union of queues with identical features.
		struct QueueFamily
		{
			// Pointer to a parent struct.
			PhysicalDevice* p_parent;
			// Properties of this queue family
			VkQueueFamilyProperties props;
			// Index of this queue family. 
			uint32_t index;

			// Queue of commands submitted to a device.
			struct Queue
			{
				// Pointer to a parent struct.
				QueueFamily* p_parent;
				// Vulkan handle of this wrap.
				VkQueue vk_handle;

				Queue(QueueFamily* p_parent, VkDeviceQueueCreateFlags flags, uint32_t queue_family_index,
					const float* p_priorities, uint32_t count);

			}; // Queue
			// Queues belonging to this family.
			vec<Queue> queues{};

		};
		// Queue families available on a physical device
		vec<QueueFamily> queue_families{};
		// Enumerates all the queue families available on this device.
		void enumerateQueueFamilies();

		// Logical device represents a logical connection to physical device.
		// It's one of the primary objects to interact with Vulkan implementation.
		struct LogicalDevice
		{
		public:
			// Vulkan handle of this wrap.
			VkDevice vk_handle{};
			// Pointer to a parent struct.
			PhysicalDevice* p_parent;
			/**
			* Logical device represents a logical connection to physical device.
			* It's one of the primary objects to interact with Vulkan implementation.
			*
			* @param PhysicalDevice* p_parent - A device to which create a connection.
			* @param uint32_t queue_count - Number of queues to be created along with device.
			* @param std::vector<VkDeviceQueueCreateInfo> const queue_infos - A vector of queue info objects for each queue. Must be of equal size that queue_count variable. If queue_count is 0, then it must be an empty vector.
			* @param std::vector<const char*> const enabled_extension_names - A vector of UTF-8 null-terminated strings of extension names to be enabled for this device. For now, it must be an empty vector.
			* @param VkPhysicalDeviceFeatures* const p_features - Determines which of parent physical device's features will be enabled for this logical device. One must not try to enable here features that are not supported by parent device.
			*
			*/
			LogicalDevice(PhysicalDevice* p_parent, uint32_t queue_count, std::vector<VkDeviceQueueCreateInfo> const queue_infos,
				std::vector<const char*> const enabled_extension_names, VkPhysicalDeviceFeatures* const p_features);

			// Command pool is a pool of memory allocated for command buffers.
			// A single command pool must NOT be used concurrently in multiple threads.
			struct CommandPool
			{
				// Pointer to a parent struct.
				LogicalDevice* p_parent;
				// Vulkan handle of this wrap.
				VkCommandPool vk_handle;
				// Pointer to a thread which owns this command pool.
				std::thread::id thread_id{};

				/**
				* Command pool is a pool of memory allocated for command buffers.
				* A single command pool must NOT be used concurrently in multiple threads.
				* Must be created from a thread that is desired to be its owner.
				* 
				* TODO - add param documentation.
				*/
				CommandPool(LogicalDevice* p_parent, QueueFamily* p_queue_family, VkCommandPoolCreateFlagBits* p_flags_bitmask,
					VkAllocationCallbacks* p_allocator);
				
				// Command buffer is an object used to record Vulkan commands, and then submit them.
				struct CommandBuffer
				{
					// Pointer to a parent struct.
					CommandPool* p_parent;
					// Vulkan handle of this wrap.
					VkCommandBuffer vk_handle;

				};
				// Command buffers allocated in this command pool.
				vec<CommandBuffer> cmd_bufs;

			};
			// Command pools created with usage of this device.
			vec<CommandPool> command_pools{};


			
			

		}; // LogicalDevice
		vec<LogicalDevice> logical_devices{};


		// Device layers available for current physical device.
		vec<VkLayerProperties> layer_props{};
		void enumerateDeviceLayers();

		VkPhysicalDeviceFeatures getFeatures();

		// Enumerates all attached graphical processing units, or other
		// devices that are recognized as suitable for rendering by Vulkan.
		static void enumerateAttached();

	private:

		PhysicalDevice(VkPhysicalDevice vk_handle);

	}; // PhysicalDevice

	static class ExternalAccess
	{
	private:
		friend class Window;
		static void addWindow(Window* window);
	};

	// Physical devices found by current Vulkan instance.
	vec<PhysicalDevice> phys_devices;

	//!// List of properties of physical device groups
	vec<VkPhysicalDeviceGroupProperties> phys_device_groups;

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
