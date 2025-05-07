
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <GLFW/glfw3.h>
#include "AppStructure.h"
#include "WindowManager.h"
#include "CorEngine.h"
#include "Types.h"
#include "Debug.h"

namespace AppStructure
{
	namespace
	{

		///----------------------
		//
		/// RE-ORGANIZE THIS HELL
		//
		///----------------------

		namespace VkHierarchyTree
		{
			//!// Vulkan instance - root of the hierarchy tree, and the basement of everything in Vulkan.
			VkInstance instance;

			//!// Physical device represents a graphical processing unit or another
			//!// device that is recognized as suitable for rendering by Vulkan.
			struct PhysicalDevice
			{
				//!// Vulkan handle of this wrap.
				VkPhysicalDevice vk_handle;
				/// Constructor
				PhysicalDevice(VkPhysicalDevice vk_handle) : vk_handle(vk_handle)
				{
					enumerateQueueFamilyProps();

					phys_device_props = new VkPhysicalDeviceProperties2{};
					vkGetPhysicalDeviceProperties2(vk_handle, phys_device_props);

					CORENGINE_DEBUG_PRINT(phys_device_props->properties.deviceName)
				}

				//!// Logical device represents a connection to a physical one.
				//!// One physical device may have several logical devices.
				struct LogicalDevice
				{
					//!// Vulkan handle of this wrap.
					VkDevice vk_handle;
					//!// Pointer to a physical device which is parent to this logical one.
					PhysicalDevice* parent_device;
					/// Constructor
					LogicalDevice(PhysicalDevice* parent, uint32_t queue_count, vec<VkDeviceQueueCreateInfo> queue_infos, vec<const char*> enabled_extension_names, VkPhysicalDeviceFeatures* features)
					{
						parent_device = parent;
						VkDeviceCreateInfo info{};
						info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
						info.queueCreateInfoCount = queue_count;
						info.pQueueCreateInfos = queue_infos.data();
						info.enabledExtensionCount = enabled_extension_names.size();
						info.ppEnabledExtensionNames = enabled_extension_names.data();
						info.pEnabledFeatures = features;
						
						assert((vkCreateDevice(parent_device->vk_handle, &info, nullptr, &vk_handle)) == VK_SUCCESS);
					}

				};

				//!// Queue of commands for this device.
				struct Queue
				{
				public:
					//!// Pointer to a parent struct
					PhysicalDevice* phys_device;
					//!// Vulkan handle of this wrap.
					VkQueue vk_handle;

					Queue(PhysicalDevice* parent, VkDeviceQueueCreateFlags flags, uint32_t queue_family_index, const float prior)
					{
						phys_device = parent;
						constructor(flags, queue_family_index, &prior, 1);
					}
					Queue(PhysicalDevice* parent, VkDeviceQueueCreateFlags flags, uint32_t queue_family_index, const float* prior, uint32_t count)
					{
						phys_device = parent;
						constructor(flags, queue_family_index, prior, count);
					}



				private:
					//!// Internal constructor.
					void constructor(VkDeviceQueueCreateFlags flags, uint32_t queue_family_index, const float* prior, uint32_t count)
					{
						if (queue_family_index <= phys_device->queue_families.size())
						{
							VkDeviceQueueCreateInfo info{};
							info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
							info.flags = flags;
							info.queueCount = count;
							info.queueFamilyIndex = queue_family_index;
							info.pQueuePriorities = prior;
						}
						else
						{
							throw std::exception("Error - Desired queue family is absent on current GPU.");
						}
					}

				};
				vec<Queue> queues;

				//!// Device layers available for current physical device.
				vec<VkLayerProperties> layer_props;
				void enumerateDeviceLayers()
				{
					uint32_t layer_count;
					assert(vkEnumerateDeviceLayerProperties(vk_handle, &layer_count, nullptr));
					if (layer_count != 0)
					{
						layer_props.reserve(layer_count);
						assert(vkEnumerateDeviceLayerProperties(vk_handle, &layer_count, layer_props.data()));
						CORENGINE_DEBUG_PRINT(layer_count)
					}
					else
					{
						throw std::exception("Error - No available layers found for current device.");
					}
				}

				//!// I don't remember what does it stands for
				VkPhysicalDeviceMemoryProperties mem_props;

				//!// Queue families available on a physical device
				vec<VkQueueFamilyProperties> queue_families{};
				void enumerateQueueFamilyProps()
				{
					uint32_t queue_family_props_count;
					vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, nullptr);

					queue_families = vec<VkQueueFamilyProperties>(queue_family_props_count);
					vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, queue_families.data());

					CORENGINE_DEBUG_PRINT(queue_family_props_count);
					CORENGINE_DEBUG_PRINT(this->queue_families.size());
				}

				//!// Pointer to a structure which stores properties of this device.
				VkPhysicalDeviceProperties2* phys_device_props;


			};
			//!// Physical devices found by current Vulkan instance.
			vec<PhysicalDevice> phys_devices;
			void enumeratePhysDevices()
			{
				uint32_t phys_devices_found;
				assert((vkEnumeratePhysicalDevices(instance, &phys_devices_found, nullptr)) == VK_SUCCESS);
				if (phys_devices_found > 0)
				{
					vec<VkPhysicalDevice> raw_devices(phys_devices_found);
					assert((vkEnumeratePhysicalDevices(instance, &phys_devices_found, raw_devices.data())) == VK_SUCCESS);
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
					throw std::exception("Error - No attached physical devices found.");
				}
			}

			//!// List of properties of physical device groups
			vec<VkPhysicalDeviceGroupProperties> phys_device_groups;
			void enumeratePhysDeviceGroupProps()
			{
				uint32_t phys_device_group_props_count;
				assert((vkEnumeratePhysicalDeviceGroups(instance, &phys_device_group_props_count, nullptr)) == VK_SUCCESS);
				if (phys_device_group_props_count != 0)
				{
					phys_device_groups.reserve(phys_device_group_props_count);
					assert((vkEnumeratePhysicalDeviceGroups(instance, &phys_device_group_props_count, phys_device_groups.data())) == VK_SUCCESS);

					CORENGINE_DEBUG_PRINT(phys_device_group_props_count);
				}
			}


		};// VkHierarchyTree

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

	using LogicalDevice = VkHierarchyTree::PhysicalDevice::LogicalDevice;

	// Creates VkDevice object alongside with a pack of queues.
	LogicalDevice createVirtualDevice(uint32_t phys_device_index, uint32_t queue_count, 
		vec<VkDeviceQueueCreateInfo> queue_infos, vec<const char*> enabled_extension_names, 
		VkPhysicalDeviceFeatures* features)
	{
		return LogicalDevice(&VkHierarchyTree::phys_devices[phys_device_index], 
			queue_count, queue_infos, enabled_extension_names, features);
	}

	void ExternalAccess::addWindow(Window* window)
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
		VkResult res = vkCreateInstance(createVkInfo(createVkAppInfo(app_name, app_version)),
			nullptr, &VkHierarchyTree::instance);
		assert(res == VK_SUCCESS);
		VkHierarchyTree::enumeratePhysDeviceGroupProps();
		VkHierarchyTree::enumeratePhysDevices();

	}

	void finalCleanup()
	{
		vkDestroyInstance(VkHierarchyTree::instance, nullptr);

		for (size_t i = 0; i < app_windows.size(); i++)
		{
			app_windows[i].reset();
		}

		glfwTerminate();
	}

}

