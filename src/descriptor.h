#pragma once

#include <vulkan/vulkan.h>

#include "shader_common.h"
#include "device2.h"

struct Descriptor_InitInfo{
	struct Device_State* device_ref;
};
struct DescriptorContext {

	struct Device_State* device_ref;
	
	VkDescriptorPool pool;

	VkDescriptorSetLayout globalLayout;
	VkDescriptorSetLayout instanceLayout;
	VkDescriptorSetLayout materialLayout;
	VkDescriptorSetLayout samplerLayout;

	// should work on load only
	VkDescriptorImageInfo descriptor_image_info_textures[MAX_TEXTURES];
	uint32_t descriptor_texture_last_used;
};

// lifecycle
void Descriptor_Init(struct Descriptor_InitInfo *info, struct DescriptorContext * context);

void Descriptor_Destroy(struct DescriptorContext * context);

// pool control
void Descriptor_ResetPool(struct DescriptorContext * context);

// // layouts access
// struct DescriptorContext* Descriptor_GetContext(
// 	);

// allocation
VkDescriptorSet Descriptor_Allocate(
	struct DescriptorContext * context
	
	, VkDescriptorSetLayout layout);

void Descriptor_SetTextureToDescriptorInfoArray(
	struct DescriptorContext * context,
	VkImageView view,
	VkSampler sampler,
	uint32_t position);

void Descriptor_UpdateTextureDescriptors(
	struct DescriptorContext * context,
	VkDescriptorSet descriptor_set,
	VkDescriptorImageInfo* arr,
	uint32_t count,
	uint32_t offset);

// updates
void Descriptor_UpdateBuffer(struct DescriptorContext * context,
	VkDescriptorSet set,
	uint32_t binding,
	VkDescriptorType type,
	VkBuffer buffer,
	VkDeviceSize size);

void Descriptor_UpdateImage(struct DescriptorContext * context,
	VkDescriptorSet set,
	uint32_t binding,
	VkDescriptorImageInfo* imageInfo);
