
#pragma once

#include "CorE/corengine.hpp"
#include "CorE/core_manager.hpp"
#include "CorE/short_type.hpp"

namespace CorE
{
	namespace Graphics
	{



		/*
		* A shader object specifies programmable operations that execute for each vertex,
		* control point, tessellated vertex, primitive, fragment, or workgroup in the
		* corresponding stage(s) of the graphics and compute pipelines.
		*/
		struct Shader
		{

			Shader(LogicalDevice* p_device, VkShaderCreateFlagsEXT flags, VkShaderStageFlagBits stage,
				VkShaderStageFlags next_stage, VkShaderCodeTypeEXT code_type, size_t code_size,
				const char* p_code, std::string name, std::vector<VkDescriptorSetLayout> desc_set_layouts,
				std::vector<VkPushConstantRange> push_constant_ranges, VkSpecializationInfo p_spec_info);
			~Shader();

			VkShaderEXT vk_handle;
			LogicalDevice* p_device;

			std::string name;

		};

		/*
		* A descriptor is an opaque data structure representing a shader
		* resource such as a buffer, buffer view, image view, sampler, or 
		* combined image sampler.
		*/
		namespace Descriptor
		{

			struct IDescriptor
			{
				VkDescriptorType type;


			};

			/*
			* A group of descriptors with certain layout.
			*/
			struct DescriptorSet
			{

				DescriptorSet(VkDescriptorSetLayoutCreateFlags flags, 
					std::vector<VkDescriptorSetLayoutBinding> bindings);

				vec<IDescriptor> descriptors;
				VkDescriptorSetLayout vk_handle;
				LogicalDevice* p_device;
			};


			/*
			* A descriptor type associated with an image resource 
			* via an image view that load, store, and atomic operations 
			* can be performed on.
			* 
			* Loads, stores and atomic operations on storage images are 
			* supported for image views whose format features contain 
			* VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT.
			*/
			struct StorageImage : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

			};

			/*
			* A descriptor type associated with a sampler object,
			* used to control the behavior of sampling operations.
			*/
			struct Sampler : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

			};

			/*
			* A descriptor type associated with an image resource
			* via an image view that sampling operations can be performed on.
			* 
			* Shaders combine a sampled image variable and a sampler variable 
			* to perform sampling operations.
			* 
			* Sampled images are supported in all shader stages for image views
			* whose format features contain VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT.
			*/
			struct SampledImage : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_SAMPLER;

			};

			/*
			* A descriptor type that combines an image and a sampler
			* into single descriptor.
			*/
			struct CombinedImageSampler : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

			};

			/*
			* A descriptor type associated with a buffer resource via
			* a buffer view that image sampling operations can be performed on.
			* 
			* Uniform texel buffers define a tightly-packed 1-dimensional 
			* linear array of texels, with texels going through format
			* conversion when read in a shader in the same way as they 
			* are for an image.
			* 
			* Load operations from uniform texel buffers are supported in 
			* all shader stages for buffer view formats which report format 
			* features support for VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT.
			*/
			struct UniformTexelBuffer : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;

			};

			/*
			* A descriptor type associated with a buffer resource via
			* buffer view that image load, store, and atomic operations 
			* can be performed on.
			* 
			* Storage texel buffers define a tightly-packed 1-dimensional
			* linear array of texels, with texels going through format
			* conversion when read in a shader in the same way as they 
			* are for an image. 
			* 
			* Unlike uniform texel buffers, these buffers can also be 
			* written to in the same way as for storage images.
			* 
			* Loads, stores and atomic operations on storage texel buffers
			* are supported for texel buffer views whose format features
			* contain VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT.
			*/
			struct StorageTexelBuffer : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;

			};

			/*
			* A descriptor type associated with a buffer resource directly,
			* described in a shader as a structure with various members
			* that load, store and atomic operations can be performed on.
			*/
			struct StorageBuffer : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

			};

			/*
			* A descriptor type associated with a buffer resource directly,
			* described in a shader as a structure with various members
			* that load, store and atomic operations can be performed on.
			*/
			struct UniformBuffer : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

			};

			/*
			* A descriptor type associated with a buffer resource directly,
			* but with offset specified in different way. The base offset 
			* calculated by the VkDescriptorBufferInfo when initially 
			* updating the descriptor set is added to a dynamic offset when
			* binding the descriptor set.
			*/
			struct DynamicUniformBuffer : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

			};

			/*
			* A descriptor type associated with a buffer resource directly,
			* but with offset specified in different way. The base offset
			* calculated by the VkDescriptorBufferInfo when initially
			* updating the descriptor set is added to a dynamic offset when
			* binding the descriptor set.
			*/
			struct DynamicStorageBuffer : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;

			};

			/*
			* A descriptor type that is similar to UniformBuffer, but takes
			* its storage from the encompassing descriptor set instead of being
			* backed by buffer memory.
			* Typically used to access a small set of constant data that doesn't
			* require the additional flexibility of a uniform buffer.
			* Compared to push constants, they allow reusing the same set of constant
			* data across multiple disjoint sets of drawing and dispatching commands.
			*/
			struct InlineUniformBlock : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;

			};

			/*
			* A descriptor type associated with an image resource via an
			* image view that can be used in weight image sampling.
			*/
			struct SampleWeightImage : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM;

			};

			/*
			* A descriptor type associated with an image resource via an
			* image view that can be used in block matching.
			*/
			struct BlockMatchImage : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM;
			
			};

			/*
			* A descriptor type associated with an image resource via an
			* image view that can be used for framebuffer local load
			* operations in a fragment shader.
			*/
			struct InputAttachment : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
			
			};

			/*
			* A descriptor type that is used to retrieve scene geometry from
			* within shaders that are used for ray traversal.
			* Shaders have read-only access to the memory.
			*/
			struct AccelerationStructure : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
			
			};

			/*
			* A descriptor type which can mutate to any of the descriptor types
			* iven in the VkMutableDescriptorTypeListEXT::pDescriptorTypes list 
			* of descriptor types in the pNext chain of VkDescriptorSetLayoutCreateInfo 
			* for this binding. 
			// TODO: fix this comment when mutables will be wrapped
			*/
			struct Mutable : IDescriptor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_MUTABLE_EXT;

			};

			/*  // Weidr thingy, by somewhat reason does not exist.
			struct StorageTensor
			{
				static constexpr VkDescriptorType type = VK_DESCRIPTOR_TYPE_TENSOR_ARM;
			};
			*/ 




		} // namespace Descriptor

	}// namespace Graphics
}// namespace CorE
