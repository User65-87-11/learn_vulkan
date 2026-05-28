#pragma once
#include "device2.h"
#include <vulkan/vulkan.h>

struct GraphicsPipelineCreateInfo {

	struct Device_State * ref_device;
	// VkDevice device;

	VkExtent2D extent;

	VkFormat colorFormat;
	VkFormat depthFormat;

	VkShaderModule vertexShader;
	VkShaderModule fragmentShader;

	VkDescriptorSetLayout* descriptorSetLayouts;
	uint32_t descriptorSetLayoutCount;
};

struct GraphicsPipeline {
	struct Device_State * ref_device;
	
	VkPipeline handle;
	VkPipelineLayout layout;

	VkShaderModule vertexShader;
	VkShaderModule fragmentShader;
};

void Pipeline_CreateGraphics( struct GraphicsPipelineCreateInfo *info, struct GraphicsPipeline* pipeline) ;

void Pipeline_Destroy( struct GraphicsPipeline* pipeline);
