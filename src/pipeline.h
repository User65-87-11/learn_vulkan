#pragma once
#include "device2.h"
#include <vulkan/vulkan.h>

struct Pipeline_Info {

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

struct Pipeline_Main {
	struct Device_State * ref_device;
	
	VkPipeline handle;
	VkPipelineLayout layout;

	VkShaderModule vertexShader;
	VkShaderModule fragmentShader;
};

void Pipeline_CreateGraphics( struct Pipeline_Info *info, struct Pipeline_Main* pipeline) ;

void Pipeline_Destroy( struct Pipeline_Main* pipeline);
