#pragma once

#include <vulkan/vulkan.h>

struct GraphicsPipeline {
    VkPipeline handle;
    VkPipelineLayout layout;
};

struct GraphicsPipelineCreateInfo {
    VkDevice device;

    VkExtent2D extent;

    VkFormat colorFormat;
    VkFormat depthFormat;

    VkShaderModule vertexShader;
    VkShaderModule fragmentShader;

    VkDescriptorSetLayout* descriptorSetLayouts;
    uint32_t descriptorSetLayoutCount;

};

void Pipeline_CreateGraphics(
   struct  GraphicsPipelineCreateInfo* info,
   struct GraphicsPipeline * pipeline
);

void Pipeline_Destroy(
    VkDevice device,
    struct GraphicsPipeline* pipeline
);