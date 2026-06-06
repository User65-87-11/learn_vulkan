#pragma once

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "resource.h"
#include "shader_common.h"
#include "device2.h"
#include "vulkan/vulkan_core.h"

// struct Descriptor_InitInfo{
// 	struct Device_State* device_ref;
// };
// struct DescriptorContext {

// 	struct Device_State* device_ref;
	
// 	VkDescriptorPool pool;

// 	VkDescriptorSetLayout globalLayout;
// 	VkDescriptorSetLayout instanceLayout;
// 	VkDescriptorSetLayout materialLayout;
// 	VkDescriptorSetLayout textureLayout;
// 	VkDescriptorSetLayout samplerLayout;

// 	// should work on load only
// 	// VkDescriptorImageInfo descriptor_image_info_textures[MAX_TEXTURES];
// 	// uint32_t descriptor_texture_last_used;
// };

// lifecycle
// void Descriptor_Init(struct Descriptor_InitInfo *info, struct DescriptorContext * context);

// void Descriptor_Destroy(struct DescriptorContext * context);

// // pool control
// void Descriptor_ResetPool(struct DescriptorContext * context);

// // layouts access
// struct DescriptorContext* Descriptor_GetContext(
// 	);

// allocation

void Descriptor_Allocate(
	VkDevice device, 
	VkDescriptorSetLayout layout,
	VkDescriptorPool pool,
	VkDescriptorSet * set,
	uint32_t count
);

void Descriptor_SetTextureToDescriptorInfoArray(
	VkDevice device,
	VkImageView view,
	VkSampler sampler,
	uint32_t position
);

void Descriptor_UpdateSamplerDescriptors(
	VkDevice device,
	VkDescriptorSet descriptor_set,
	uint32_t binding,
	VkSampler sampler) ;

void Descriptor_UpdateTextureDescriptors(
	VkDevice device,
	VkDescriptorSet descriptor_set,
	uint32_t binding,
	VkImageView view,
	uint32_t offset);

// updates
void Descriptor_UpdateBuffer(
	VkDevice device,
	VkDescriptorSet set,
	uint32_t binding,
	VkDescriptorType type,
	VkBuffer buffer,
	VkDeviceSize size,
	uint32_t offset
);

void Descriptor_UpdateImage(
	VkDevice device,
	VkDescriptorSet set,
	uint32_t binding,
	VkDescriptorImageInfo* imageInfo
);
