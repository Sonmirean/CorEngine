
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
			struct LogicalDevice
			{
				VkDevice vk_handle;


			};
			
			struct PhysicalDevice
			{
			public:


				vec<VkQueueFamilyProperties>	 queue_family_props;


				void defineQueueFamilyProps()
				{
					uint32_t queue_family_props_count;
					vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, nullptr);
					queue_family_props.reserve(queue_family_props_count);
					vkGetPhysicalDeviceQueueFamilyProperties(vk_handle, &queue_family_props_count, queue_family_props.data());

					CORENGINE_DEBUG_PRINT(queue_family_props_count);
				}


				PhysicalDevice(VkPhysicalDevice vk_handle) : vk_handle(vk_handle)
				{
					defineQueueFamilyProps();

					CORENGINE_DEBUG_PRINT(queue_family_props.size())

					/// 
					for (size_t i = 0; i < queue_family_props.size(); i++)
					{
						CORENGINE_DEBUG_PRINT(queue_family_props[i].queueFlags);
					}
				}

				struct Queue
				{
				public:
					PhysicalDevice* phys_device;
					VkQueue vk_handle;

					Queue(VkDeviceQueueCreateFlags flags, uint32_t queu_family_index)
					{
						constructor(flags, queu_family_index, 1);
					}
					Queue(VkDeviceQueueCreateFlags flags, uint32_t queu_family_index, uint32_t count)
					{
						constructor(flags, queu_family_index, count);
					}

					

				private:
					void constructor(VkDeviceQueueCreateFlags flags, uint32_t queu_family_index, uint32_t count)
					{
						if (queu_family_index <= phys_device->queue_family_props.size())
						{
							VkDeviceQueueCreateInfo info{};
							info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
							info.flags = flags;
							info.queueCount = count;
							info.queueFamilyIndex = queu_family_index;
						}
						else
						{
							throw std::out_of_range("Error - Desired queue family is absent on current GPU.");
						}
					}

				};
				struct Heap
				{
					PhysicalDevice* phys_device;

					VkMemoryHeap vk_handle;


				};



			private:
				VkPhysicalDevice vk_handle;

				
				
				VkPhysicalDeviceMemoryProperties mem_props;

				vec<Queue> vk_queues;
				vec<Heap>  vk_heaps;


			};

			VkInstance			  instance;
			VkAllocationCallbacks alloc_callbacks;
			vec<PhysicalDevice>		  phys_gpus;

			void enumeratePhysDevices()
			{
				
				uint32_t phys_devices_found;
				checkVkSuccess(vkEnumeratePhysicalDevices(instance, &phys_devices_found, nullptr));

				if (phys_devices_found > 0)
				{
					vec<VkPhysicalDevice> raw_devices(phys_devices_found);

					checkVkSuccess(vkEnumeratePhysicalDevices(instance, &phys_devices_found, raw_devices.data()));

					phys_gpus.reserve(phys_devices_found);

					for (unsigned int i = 0; i < phys_devices_found; i++)
					{
						PhysicalDevice phys_device = PhysicalDevice(raw_devices[i]);
						phys_gpus.push_back(phys_device);
					}
					CORENGINE_DEBUG_PRINT(phys_devices_found)
					
				}
				else
				{
					throw std::runtime_error("Error - No attached physical devices found.");
				}
			}


		};

		vec<sptr<Window>>			 app_windows;

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
		VkInstanceCreateInfo* redefineVkInfo(VkInstanceCreateInfo* c_info, VkApplicationInfo app_info)
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
		VkApplicationInfo redefineVkAppInfo(const VkApplicationInfo* a_info)
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

		
		void enumerateQueueFamilyProps(VkPhysicalDevice device)
		{
			uint32_t count;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

		}

		VkQueue createQueue(VkPhysicalDevice* gpu_ptr)
		{
			
		}

		void initCommandPool()
		{

		}

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
		checkVkSuccess(res);
		VkHierarchyTree::enumeratePhysDevices();
	}

	void initVulkan(VkInstanceCreateInfo* info, VkAllocationCallbacks* callbacks)
	{
		VkResult res = vkCreateInstance(redefineVkInfo(info, redefineVkAppInfo(info->pApplicationInfo)),
			callbacks, &VkHierarchyTree::instance);
		checkVkSuccess(res);
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

