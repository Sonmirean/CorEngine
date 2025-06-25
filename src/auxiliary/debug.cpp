
#include "debug.hpp"

void ensureVkSuccess(VkResult res)
{
	if (res != VK_SUCCESS)
	{
		std::cout << "Vulkan function thrown an error. VkResult: " << string_VkResult(res) << std::endl;
		throw std::exception("Vulkan function thrown an error.");
	}
}