
#include "Debug.h"

void ensureVkSuccess(VkResult res)
{
	if (res != VK_SUCCESS)
	{
		std::cout << "Vulkan function thrown an error. VkResult code: " << res << std::endl;
		throw std::exception("Vulkan function thrown an error.");
	}
}

void printPhysicalDeviceProps(VkPhysicalDeviceProperties* props)
{
	std::cout \
		<< "\033[35m\n===========================================\n"
		<< "\033[36m >>> DEVICE INFO - " << props->deviceName
		<< "\n" "API version: " << props->apiVersion
		<< "\n" "Driver version: " << props->driverVersion
		<< "\n" "Vendor ID: " << props->vendorID
		<< "\n" "Vendor device ID: " << props->deviceID
		<< "\n" "Device type: " << props->deviceType
		<< "\n" "Pipeline cache UUID: " << props->pipelineCacheUUID

		<< "\n------------------------------\n" 

		"\033[36m DEVICE LIMITS: "
		<< "\n" "Maximum image dimensions: " 
		"1D: " << props->limits.maxImageDimension1D <<
		" 2D: " << props->limits.maxImageDimension2D <<
		" 3D: " << props->limits.maxImageDimension3D <<
		" Cube: " << props->limits.maxImageDimensionCube
		<< "\n" "Maximum image array layers: " << props->limits.maxImageArrayLayers
		<< "\n" "Maximum texel buffer elements: " << props->limits.maxTexelBufferElements
		<< "\n" "Maximum uniform buffer range: " << props->limits.maxUniformBufferRange
		<< "\n" "Maximum storage buffer range: " << props->limits.maxStorageBufferRange
		<< "\n" "Maximum push constant pool size: " << props->limits.maxPushConstantsSize
		<< "\n" "Maximum memory allocations: " << props->limits.maxMemoryAllocationCount
		<< "\n" "Maximum sampler allocations: " << props->limits.maxSamplerAllocationCount

		<< "\n" "Buffer image granularity: " << props->limits.bufferImageGranularity
		<< "\n" "Sparse address space available: " << props->limits.sparseAddressSpaceSize
		<< "\n" "Maximum descriptor sets per pipeline: " << props->limits.maxBoundDescriptorSets

		<< "\n" "Maximum samplers per shader stage: " << props->limits.maxPerStageDescriptorSamplers
		<< "\n" "Maximum uniform buffers per shader stage: " << props->limits.maxPerStageDescriptorUniformBuffers
		<< "\n" "Maximum storage buffers per shader stage: " << props->limits.maxPerStageDescriptorStorageBuffers
		<< "\n" "Maximum sampled images per shader stage: " << props->limits.maxPerStageDescriptorSampledImages
		<< "\n" "Maximum storage images per shader stage: " << props->limits.maxPerStageDescriptorStorageImages
		<< "\n" "Maximum input attachments per shader stage: " << props->limits.maxPerStageDescriptorInputAttachments
		<< "\n" "Maximum resources per shader stage: " << props->limits.maxPerStageResources

		<< "\n" "Maximum samplers per pipeline layout: " << props->limits.maxDescriptorSetSamplers
		<< "\n" "Maximum uniform buffers per pipeline layout: " << props->limits.maxDescriptorSetUniformBuffers
		<< "\n" "Maximum dynamic uniform buffers per pipeline layout: " << props->limits.maxDescriptorSetUniformBuffersDynamic
		<< "\n" "Maximum storage buffers per pipeline layout: " << props->limits.maxDescriptorSetStorageBuffers
		<< "\n" "Maximum dynamic storage buffers per pipeline layout: " << props->limits.maxDescriptorSetStorageBuffersDynamic
		<< "\n" "Maximum sampled images per pipeline layout: " << props->limits.maxDescriptorSetSampledImages
		<< "\n" "Maximum storage images per pipeline layout: " << props->limits.maxDescriptorSetStorageImages
		<< "\n" "Maximum input attachments per pipeline layout: " << props->limits.maxDescriptorSetInputAttachments

		<< "\n" "Maximum vertex input attributes per graphics pipeline: " << props->limits.maxVertexInputAttributes
		<< "\n" "Maximum vertex buffers per graphics pipeline: " << props->limits.maxVertexInputBindings
		<< "\n" "Maximum vertex input attribute offset per vertex input binding stride: " << props->limits.maxVertexInputAttributeOffset
		<< "\n" "Maximum vertex input binding stride: " << props->limits.maxVertexInputBindingStride
		<< "\n" "Maximum vertex shader output components: " << props->limits.maxVertexOutputComponents

		<< "\n" "Maximum tessellation generation level: " << props->limits.maxTessellationGenerationLevel
		<< "\n" "Maximum patch size (in vertices) for tessellation control shader: " << props->limits.maxTessellationPatchSize
		<< "\n" "Maximum per vertex input variables to the tessellation control shader stage: " << props->limits.maxTessellationControlPerVertexInputComponents
		<< "\n" "Maximum per vertex output variables from the tessellation control shader stage: " << props->limits.maxTessellationControlPerVertexOutputComponents
		<< "\n" "Maximum per patch output variables to the tessellation control shader stage: " << props->limits.maxTessellationControlPerPatchOutputComponents
		<< "\n" "Maximum total output variables to the tessellation control shader stage: " << props->limits.maxTessellationControlTotalOutputComponents
		<< "\n" "Maximum per vertex input variables to the tessellation evaluation shader stage: " << props->limits.maxTessellationEvaluationInputComponents
		<< "\n" "Maximum per vertex output variables from the tessellation evaluation shader stage: " << props->limits.maxTessellationEvaluationOutputComponents

		<< "\n" "Maximum geometry shader invocations: " << props->limits.maxGeometryShaderInvocations
		<< "\n" "Maximum geometry shader stage input variables: " << props->limits.maxGeometryInputComponents
		<< "\n" "Maximum geometry shader stage output variables: " << props->limits.maxGeometryOutputComponents
		<< "\n" "Maximum geometry shader stage output vertices: " << props->limits.maxGeometryOutputVertices
		<< "\n" "Maximum geometry shader stage total output components: " << props->limits.maxGeometryTotalOutputComponents

		<< "\n" "Maximum fragment shader stage input variables: " << props->limits.maxFragmentInputComponents
		<< "\n" "Maximum fragment shader stage output attachments: " << props->limits.maxFragmentOutputAttachments
		<< "\n" "Maximum fragment shader stage dual-source blending attachments: " << props->limits.maxFragmentDualSrcAttachments
		<< "\n" "Maximum fragment shader stage combined resources: " << props->limits.maxFragmentCombinedOutputResources

		<< "\n" "Maximum compute shader stage shared memory size: " << props->limits.maxComputeSharedMemorySize
		<< "\n" "Maximum compute workgroups per dispatching command: "
		"X: " << props->limits.maxComputeWorkGroupCount[0] <<
		" Y: " << props->limits.maxComputeWorkGroupCount[1] <<
		" Z: " << props->limits.maxComputeWorkGroupCount[2]
		<< "\n" "Maximum compute shader invocations per workgroup: " << props->limits.maxComputeWorkGroupInvocations
		<< "\n" "Maximum compute workgroup size: "
		"X: " << props->limits.maxComputeWorkGroupSize[0] <<
		" Y: " << props->limits.maxComputeWorkGroupSize[1] <<
		" Z: " << props->limits.maxComputeWorkGroupSize[2]

		<< "\n" "Subpixel precision bits: " << props->limits.subPixelPrecisionBits
		<< "\n" "Subtexel precision bits: " << props->limits.subTexelPrecisionBits
		<< "\n" "Mipmap precision bits: " << props->limits.mipmapPrecisionBits
		<< "\n" "Maximum indexed draw value: " << props->limits.maxDrawIndexedIndexValue
		<< "\n" "Maximum indirect draw count: " << props->limits.maxDrawIndirectCount
		<< "\n" "Maximum sampler LOD bias: " << props->limits.maxSamplerLodBias
		<< "\n" "Maximum sampler anisotropy degree: " << props->limits.maxSamplerAnisotropy

		<< "\n" "Maximum viewports: " << props->limits.maxViewports
		<< "\n" "Maximum viewport dimensions: "
		"X: " << props->limits.maxViewportDimensions[0] <<
		" Y: " << props->limits.maxViewportDimensions[1]
		<< "\n" "Viewport bounds range: "
		"Min: " << props->limits.maxViewportDimensions[0] <<
		" Max: " << props->limits.maxViewportDimensions[1]
		<< "\n" "Viewport bounds subpixel precision bits: " << props->limits.viewportSubPixelBits

		<< "\n" "Minimum host address space memory alignment: " << props->limits.minMemoryMapAlignment
		<< "\n" "Minimum texel buffer offset alignment: " << props->limits.minTexelBufferOffsetAlignment
		<< "\n" "Minimum uniform buffer offset alignment: " << props->limits.minUniformBufferOffsetAlignment
		<< "\n" "Minimum storage buffer offset alignment: " << props->limits.minStorageBufferOffsetAlignment
		<< "\n" "Minimum texel offset: " << props->limits.minTexelOffset
		<< "\n" "Maximum texel offset: " << props->limits.maxTexelOffset
		<< "\n" "Minimum texel gather offset: " << props->limits.minTexelGatherOffset
		<< "\n" "Maximum texel gather offset: " << props->limits.maxTexelGatherOffset
		<< "\n" "Minimum interpolation offset: " << props->limits.minInterpolationOffset
		<< "\n" "Maximum interpolation offset: " << props->limits.maxInterpolationOffset
		<< "\n" "Subpixel interpolation offset bits: " << props->limits.subPixelInterpolationOffsetBits

		<< "\n" "Maximum framebuffer width: " << props->limits.maxFramebufferWidth
		<< "\n" "Maximum framebuffer height: " << props->limits.maxFramebufferHeight
		<< "\n" "Maximum framebuffer layers: " << props->limits.maxFramebufferLayers
		<< "\n" "Framebuffer color sample counts bitmask: " << props->limits.framebufferColorSampleCounts
		<< "\n" "Framebuffer depth sample counts bitmask: " << props->limits.framebufferDepthSampleCounts
		<< "\n" "Framebuffer stencil sample counts bitmask: " << props->limits.framebufferStencilSampleCounts
		<< "\n" "Framebuffer no attachment sample counts bitmask: " << props->limits.framebufferNoAttachmentsSampleCounts

		<< "\n" "Maximum color attachments per subpass: " << props->limits.maxColorAttachments
		<< "\n" "Sampled image sample counts bitmask (non-integer color): " << props->limits.sampledImageColorSampleCounts
		<< "\n" "Sampled image sample counts bitmask (integer color): " << props->limits.sampledImageIntegerSampleCounts
		<< "\n" "Sampled image sample counts bitmask (depth format): " << props->limits.sampledImageDepthSampleCounts
		<< "\n" "Sampled image sample counts bitmask (stencil format): " << props->limits.sampledImageStencilSampleCounts
		<< "\n" "Sampled image sample counts (storage image): " << props->limits.storageImageSampleCounts
		<< "\n" "Maximum sample mask words: " << props->limits.maxSampleMaskWords
		<< "\n" "Compute & graphics queue timestamp supported: " << props->limits.timestampComputeAndGraphics

		<< "\n" "Maximum clip distances per shader stage: " << props->limits.maxClipDistances
		<< "\n" "Maximum cull distances per shader stage: " << props->limits.maxCullDistances
		<< "\n" "Maximum combined clip & cull distances per shader stage: " << props->limits.maxCombinedClipAndCullDistances
		<< "\n" "Discrete queue priorities: " << props->limits.discreteQueuePriorities
		<< "\n" "Point size range: "
		"Min: " << props->limits.pointSizeRange[0] <<
		" Max: " << props->limits.pointSizeRange[1]
		<< "\n" "Line width range: "
		"Min: " << props->limits.lineWidthRange[0] <<
		" Max: " << props->limits.lineWidthRange[1]
		<< "\n" "Point size granularity: " << props->limits.pointSizeGranularity
		<< "\n" "Line width granularity: " << props->limits.lineWidthGranularity
		<< "\n" "Strict line rasterization: " << props->limits.strictLines
		<< "\n" "Standard rasterization sample locations: " << props->limits.standardSampleLocations
		<< "\n" "Optimal copy buffer offset alignment: " << props->limits.optimalBufferCopyOffsetAlignment
		<< "\n" "Optimal copy buffer row pitch alignment: " << props->limits.optimalBufferCopyRowPitchAlignment
		<< "\n" "Non-coherent atom size: " << props->limits.nonCoherentAtomSize
		<< "\033[35m\n===========================================\n"
		<< std::endl;
}