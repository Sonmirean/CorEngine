
#include "CorE/debug.hpp"

void ensureVkSuccess(VkResult res, std::string message)
{
	if (res != VK_SUCCESS)
	{
		std::cout << message << " VkResult code: " << string_VkResult(res) << std::endl;
		throw std::exception("Vulkan function thrown an error.");
	}
}
