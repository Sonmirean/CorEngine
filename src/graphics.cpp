
#include "CorE/graphics.hpp"

namespace CorE
{
	namespace Graphics
	{
		Shader::Shader(LogicalDevice* p_device, VkShaderCreateFlagsEXT flags, VkShaderStageFlagBits stage,
			VkShaderStageFlags next_stage, VkShaderCodeTypeEXT code_type, size_t code_size,
			const char* p_code, std::string name, std::vector<VkDescriptorSetLayout> desc_set_layouts,
			std::vector<VkPushConstantRange> push_constant_ranges, VkSpecializationInfo p_spec_info)
			: p_device(p_device), name(name)
		{
			VkShaderCreateInfoEXT info{};
			info.sType = VK_STRUCTURE_TYPE_SHADER_CREATE_INFO_EXT;
			info.codeSize = code_size;
			info.pCode = p_code;
			info.flags = flags;
			info.stage = stage;
			info.nextStage = next_stage;
			info.codeType = code_type;
			info.pName = name.c_str();
			info.setLayoutCount = static_cast<uint32_t>(desc_set_layouts.size());
			info.pSetLayouts = desc_set_layouts.data();
			info.pushConstantRangeCount = static_cast<uint32_t>(push_constant_ranges.size());
			info.pPushConstantRanges = push_constant_ranges.data();
			info.pSpecializationInfo = &p_spec_info;


			ensureVkSuccess(vkCreateShadersEXT(p_device->vk_handle, 1, &info,
				nullptr, &vk_handle), "Failed to create shader.");
		}
		Shader::~Shader()
		{
			
		}
	} // namespace Graphics
} // namespace CorE

CorE::Graphics::Descriptor::DescriptorSet::DescriptorSet(VkDescriptorSetLayoutCreateFlags flags, 
	std::vector<VkDescriptorSetLayoutBinding> bindings)
{
	VkDescriptorSetLayoutCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	create_info.flags = flags;
	create_info.bindingCount = static_cast<uint32_t>(bindings.size());
	create_info.pBindings = bindings.data();

	ensureVkSuccess(vkCreateDescriptorSetLayout(p_device->vk_handle, &create_info, nullptr,
		&vk_handle), "Failed to create descriptor set layout.");
}
