
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

		namespace VkHierarchyTree
		{
			//!// Vulkan instance - root of the hierarchy tree, and the basement of everything in Vulkan.
			VkInstance instance;

			//!// Physical device represents a graphical processing unit or another
			//!// device that is recognized as suitable for rendering by Vulkan.
			struct PhysicalDevice
			{
			public:
				//!// Vulkan handle of this wrap.
				VkPhysicalDevice vk_handle;
				/// Constructor
				PhysicalDevice(VkPhysicalDevice vk_handle) : vk_handle(vk_handle)
				{
					enumerateQueueFamilyProps();
					CORENGINE_DEBUG_PRINT(queue_family_props.size())
				}

				//!// Logical device represents a connection to a physical one.
				//!// One physical device may have several logical devices.
				struct LogicalDevice
				{
				public:
					//!// Vulkan handle of this wrap.
					VkDevice vk_handle;
					//!// Pointer to a physical device which is parent to this logical one.
					PhysicalDevice* parent_device;
					/// Constructor
					LogicalDevice()
					{

					}

				private:
					// Internal constructor
					void constructor()
					{

					}

				};
				
				//!// Queue families available on a physical device
				vec<VkQueueFamilyProperties> queue_family_props;
				void enumerateQueueFamilyProps()
				{
					uint32_t queue_family_props_count;
					vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, nullptr);
					queue_family_props.reserve(queue_family_props_count);
					vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, queue_family_props.data());
					CORENGINE_DEBUG_PRINT(queue_family_props_count);
				}

				//!// Queue of commands for this device.
				struct Queue
				{
				public:
					PhysicalDevice* phys_device;
					VkQueue vk_handle;

					Queue(VkDeviceQueueCreateFlags flags, uint32_t queue_family_index)
					{
						constructor(flags, queue_family_index, 1);
					}
					Queue(VkDeviceQueueCreateFlags flags, uint32_t queue_family_index, uint32_t count)
					{
						constructor(flags, queue_family_index, count);
					}



				private:
					void constructor(VkDeviceQueueCreateFlags flags, uint32_t queue_family_index, uint32_t count)
					{
						if (queue_family_index <= phys_device->queue_family_props.size())
						{
							VkDeviceQueueCreateInfo info{};
							info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
							info.flags = flags;
							info.queueCount = count;
							info.queueFamilyIndex = queue_family_index;
						}
						else
						{
							throw std::out_of_range("Error - Desired queue family is absent on current GPU.");
						}
					}

				};
				vec<Queue> queues;

				//!// I don't remember what does it stands for
				VkPhysicalDeviceMemoryProperties mem_props;

			};
			//!// Physical devices found by current Vulkan instance.
			vec<PhysicalDevice> phys_devices;
			void enumeratePhysDevices()
			{
				uint32_t phys_devices_found;
				checkVkSuccess_(vkEnumeratePhysicalDevices(instance, &phys_devices_found, nullptr));
				if (phys_devices_found > 0)
				{
					vec<VkPhysicalDevice> raw_devices(phys_devices_found);
					checkVkSuccess_(vkEnumeratePhysicalDevices(instance, &phys_devices_found, raw_devices.data()));
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
					throw std::runtime_error("Error - No attached physical devices found.");
				}
			}

			//!// List of properties of physical device groups
			vec<VkPhysicalDeviceGroupProperties> phys_device_groups;
			void enumeratePhysDeviceGroupProps()
			{
				uint32_t phys_device_group_props_count;
				checkVkSuccess_(vkEnumeratePhysicalDeviceGroups(instance, &phys_device_group_props_count, nullptr));
				if (phys_device_group_props_count != 0)
				{
					phys_device_groups.reserve(phys_device_group_props_count);
					checkVkSuccess_(vkEnumeratePhysicalDeviceGroups(instance, &phys_device_group_props_count, phys_device_groups.data()));

					CORENGINE_DEBUG_PRINT(phys_device_group_props_count);
				}
			}



		};

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
		checkVkSuccess_(res);
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

