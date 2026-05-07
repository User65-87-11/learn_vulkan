#pragma once

#include <vulkan/vulkan.h>

struct DescriptorContext {
    VkDescriptorPool pool;

    VkDescriptorSetLayout globalLayout;
    VkDescriptorSetLayout instanceLayout;
    VkDescriptorSetLayout materialLayout;
    VkDescriptorSetLayout samplerLayout;
};



// lifecycle
void Descriptor_Init();

void Descriptor_Shutdown();

// pool control
void Descriptor_ResetPool();

// layouts access
struct DescriptorContext* Descriptor_GetContext();

// allocation
VkDescriptorSet Descriptor_Allocate(VkDescriptorSetLayout layout);

// updates
void Descriptor_UpdateBuffer(
    VkDescriptorSet set,
    uint32_t binding,
    VkDescriptorType type,
    VkBuffer buffer,
    VkDeviceSize size
);

void Descriptor_UpdateImage(
    VkDescriptorSet set,
    uint32_t binding,
    VkDescriptorImageInfo* imageInfo
);


