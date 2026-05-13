#pragma once

#include <vulkan/vulkan.h>
#include "shader_common.h"

struct DescriptorContext {
    VkDescriptorPool pool;

    VkDescriptorSetLayout globalLayout;
    VkDescriptorSetLayout instanceLayout;
    VkDescriptorSetLayout materialLayout;
    VkDescriptorSetLayout samplerLayout;

    //should work on load only
    VkDescriptorImageInfo descriptor_image_info_textures[MAX_TEXTURES];
    uint32_t descriptor_texture_last_used ;
};



// lifecycle
void Descriptor_Init();

void Descriptor_Destroy();

// pool control
void Descriptor_ResetPool();

// layouts access
struct DescriptorContext* Descriptor_GetContext();

// allocation
VkDescriptorSet Descriptor_Allocate(VkDescriptorSetLayout layout);

void Descriptor_SetTextureToDescriptorInfoArray(
	VkImageView view,
	VkSampler sampler,
	uint32_t position
);
void Descriptor_UpdateTextureDescriptors(
	VkDescriptorSet descriptor_set, 
	VkDescriptorImageInfo * arr,
	uint32_t count,
	uint32_t offset
);
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


