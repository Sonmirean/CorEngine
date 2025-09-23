#pragma once

#include <thread>

#define CORENGINE_USE_PLATFORM_WIN32
#include "CorE/auxiliary/corengine.hpp"

#include "CorE/engine/loop_manager.hpp"
#include "CorE/engine/windowing/window_manager.hpp"
#include "CorE/auxiliary/short_type.hpp"
#include "CorE/auxiliary/debug.hpp"

namespace CorE
{

	struct CommandBuffer;
	struct Queue;
	struct PhysicalDevice;

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

		// Gets vector of queues of this family.
		std::vector<Queue> getQueues();

		// Gets properties of this queue family.
		VkQueueFamilyProperties getProps();

	private:
		friend struct PhysicalDevice;
		friend struct LogicalDevice;
		friend struct CommandPool;
		friend struct Queue;

		// Queues belonging to this family.
		vec<Queue> queues;

		// Properties of this queue family
		VkQueueFamilyProperties props;
		// Index of this queue family. 
		uint32_t index;

		// Constructor for internal use.
		QueueFamily(PhysicalDevice* p_parent, VkQueueFamilyProperties* p_props, uint32_t index);

	}; // struct QueueFamily

	/**
	* Physical display device.
	* Used to render directly to connected display.
	* Display must be free, i.e. must not be used by any OS or anything.
	*/
	struct Display
	{
	private:
		friend class PhysicalDevice;

		PhysicalDevice* p_parent;

		VkDisplayPropertiesKHR props;

		Display(VkDisplayPropertiesKHR props, PhysicalDevice* p_phys_device);

	}; // struct Display

	/**
	* Physical device represents a connection to a graphical processing unit
	* or another device that is recognized as suitable for rendering by Vulkan.
	* One must not try to create any objects with this struct manually. To query
	* the list of available physical devices, call PhysicalDevice::enumerateAll().
	*/
	struct PhysicalDevice
	{
	public:

		// Enumerates all attached graphical processing units, or other
		// devices that are recognized as suitable for rendering by Vulkan.
		static void enumerateAll();

		// Enumerates all displays and stores their handles into PhysicalDevice::displays vector.
		void enumerateDisplays();

		// Displays to which this device is connected.
		std::vector<Display> displays{};

		// Gets features of that device.
		VkPhysicalDeviceFeatures getFeatures();

		// Gets logical devices created by this physical device.
		std::vector<LogicalDevice> getLogicalDevices();

		// Gets queue families of this device.
		std::vector<QueueFamily> getQueueFamilies();

	private:
		friend struct LogicalDevice;
		friend struct PhysicalDeviceGroup;
		// Constructor for internal use.
		PhysicalDevice(VkPhysicalDevice vk_handle);

		// Logical devices created with this physical device as a base.
		vec<LogicalDevice> logical_devices;
		// Queue families available on a physical device.
		// Grouped in proper order.
		vec<QueueFamily> queue_families;
		// Device layers available for current physical device.
		vec<VkLayerProperties> layer_props;

		// Enumerates all available layers for this device.
		void enumerateDeviceLayers();
		// Enumerates all the queue families available on this device.
		void enumerateQueueFamilies();

		// Vulkan handle of this wrap.
		VkPhysicalDevice vk_handle;

	}; // struct PhysicalDevice


	/**
	* Command pool is a pool of memory from which command buffers are allocated.
	* A single command pool must NOT be used concurrently in multiple threads.
	*/
	struct CommandPool
	{
	public:

		// Returns unused memory to the system.
		// Does not affect allocated buffers.
		// May be expensive, do not use frequently.
		void trim();

		/**
		* Returns back all the resources from all the
		* command buffers allocated from this pool.
		*
		* @param VkCommandPoolResetFlags flags - Specifies behavior of reset. For now, it must be 0x00000001.
		*/
		void reset(VkCommandPoolResetFlags flags);

		/**
		* Allocates one or more command buffers.
		*
		* @param std::vector<VkCommandBufferLevel> levels - Specifies levels for eacn command buffer group.
		* @param std::vector<uint32_t> quantities - Quantities of buffers on each level. If NULL, only one
		buffer will be created for each level specified.
		*/
		void allocBuffers(std::vector<VkCommandBufferLevel> levels, std::vector<uint32_t> quantities);

		/**
		* Frees one or more command buffers.
		*
		* @param std::vector<uint32_t> buffer_indices - Indices of buffers to free.
		*/
		void freeBuffers(std::vector<uint32_t> buffer_indices);

		/**
		* Command pool is a pool of memory allocated for command buffers.
		* A single command pool must NOT be used concurrently in multiple threads.
		* Must be created from a thread that is desired to be its owner.
		*
		* @param LogicalDevice* p_parent - Pointer to a parent logical device.
		* @param QueueFamily* p_queue_family - Pointer to a queue family to which all the buffers of this pool must be submitted.
		* @param VkCommandPoolCreateFlagBits flags_bitmask - A bitmask indicating behavior of pool and buffers.
		* @param const VkAllocationCallbacks* p_allocator - Custom memory allocator.
		*/
		CommandPool(LogicalDevice* p_parent, QueueFamily* p_queue_family,
			VkCommandPoolCreateFlagBits flags_bitmask, const VkAllocationCallbacks* p_allocator);

		~CommandPool();

		// Gets queue family of this pool.
		QueueFamily getQueueFamily();

		// Gets ID of the owner thread.
		std::thread::id getThreadID();

		// Gets a list of command buffers allocated from this pool.
		std::vector<CommandBuffer> getCommandBuffers();

	private:
		// Pointer to a parent struct.
		LogicalDevice* p_parent;
		// Vulkan handle of this wrap.
		VkCommandPool vk_handle;
		// Pointer to a queue family.
		QueueFamily* p_queue_family;

		// Flags specifying behavior of the pool and its buffers.
		VkCommandPoolCreateFlags flags;

		// Pointer to a thread which owns this command pool.
		std::thread::id thread_id;
		// Command buffers allocated from this command pool.
		vec<CommandBuffer> command_buffers;


	}; //struct CommandPool

	/**
	* Command buffer is an object used to record and submit Vulkan commands.
	*/
	struct CommandBuffer
	{
	public:

		// Begins recording of a command buffer.
		void begin(VkCommandBufferUsageFlags flags, VkCommandBufferInheritanceInfo* p_inherit_info);
		// Ends recording of a command buffer.
		void end();

		// Adds one or more secondary command buffers.
		void chain(std::vector<CommandBuffer*> buffers);

	private:
		friend struct CommandPool;

		CommandBuffer(VkCommandBuffer vk_handle, CommandPool* p_parent, uint32_t index);

		// Index of a buffer in pool.
		uint32_t index;
		// Pointer to a parent struct.
		CommandPool* p_parent;
		// Vulkan handle of this wrap.
		VkCommandBuffer vk_handle;

		// Pointer to the next buffer in the chain.
		// Is nullptr if the buffer is last in the chain.
		CommandBuffer* p_next;
		// Pointer to the previous buffer in the chain.
		// Is nullptr if the buffer is primary.
		CommandBuffer* p_prev;
	};

	/**
	* Queue of commands submitted to a device.
	*/
	struct Queue
	{
	public:

		/*
		* Semaphore is a synchronization primitive used to create dependency
		* between queue operations or queue operation and the host.
		*
		* There are 2 types of semaphores - binary and timeline.
		* Binary semaphore can be signaled or unsignaled.
		* Timeline semaphore have strictly increasing uint64_t value.
		*/
		struct Semaphore
		{
			VkSemaphore vk_handle;

			//Semaphore(VkSemaphoreType type, uint64_t initial_value, ); TODO

			// Gets the type of this semaphore.
			VkSemaphoreType getType();

		private:
			VkSemaphoreType type;

		};

		// Gets family of this queue.
		QueueFamily getQueueFamily();
		// Gets index of this queue in a family.
		uint32_t getIndex();

		// Submits command buffers to this queue.
		void submitBuffers(std::vector<CommandBuffer> buffers, std::vector<VkSubmitFlags> flags); // TODO


	private:
		friend struct LogicalDevice;
		// Pointer to a parent struct.
		QueueFamily* p_parent;
		// Vulkan handle of this wrap.
		VkQueue vk_handle;

		// Index of this queue in a family.
		uint32_t index;

		// Global priority of a queue.
		VkQueueGlobalPriority global_prior;

		Queue(QueueFamily* p_parent);

	}; // struct Queue


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
		LogicalDevice(PhysicalDevice* p_parent, VkDeviceCreateInfo info, VkAllocationCallbacks allocator);



	private:
		friend struct CommandPool;

		// Pointer to a parent struct.
		PhysicalDevice* p_parent;
		// Vulkan handle of this wrap.
		VkDevice vk_handle;

		// Command pools created with usage of this device.
		vec<CommandPool> command_pools{};

	}; // struct LogicalDevice

	/**
	* This struct represents a group of physical devices of the same vendor
	* which can be represented as a single logical device to combine their memory.
	*
	*/
	struct PhysicalDeviceGroup
	{
		// Enumerates all available groups of devices.
		static void enumerateAll();

	private:
		// List of a pointers to each physical device of the group.
		vec<PhysicalDevice*> p_physical_devices;
		// Properties of this device group.
		VkPhysicalDeviceGroupProperties* p_props;

		PhysicalDeviceGroup(VkPhysicalDeviceGroupProperties props);
	};

	/**
	* This static struct represents an application built with this library.
	* Creation of any objects with it, as well as inheritance of it
	* is considered as an undefined behavior.
	*/
	struct Application
	{
		friend class Window;
		/**
		* General technical parameters submitted to an
		* application right in the moment of creation.
		*/
		struct Parameters
		{

		};
		// Parameters of the application.
		static const Parameters param;

		// Vulkan instance - basement of everything in Vulkan.
		static VkInstance instance;
		// Physical devices found by current Vulkan instance.
		static vec<PhysicalDevice> phys_devices;
		// List of physical device groups.
		static vec<PhysicalDeviceGroup> phys_device_groups;

		// Windows of this application.
		static vec<CorE::Window*> app_windows;


		// Returns quantity of app windows.
		static size_t getWinQuantity();

		// Creates Vulkan instance and initializes essentials.
		static void initVulkan(const char* app_name, uint32_t app_version[4]);



		// Final cleanup that must be called every time an application ends its session.
		static void finalCleanup();

		Application() = delete;

		static void inline checkVkInstance()
		{
			if (instance == nullptr)
			{
				throw std::runtime_error("This operation cannot be performed before initializing Vulkan.");
			}
		}

	}; // struct Application

}
