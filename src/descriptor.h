#pragma once

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "resource.h"
#include "shader_common.h"
#include "device.h"
#include "vulkan/vulkan_core.h"


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

void Descriptor_UpdateBufferImageDescriptors(
	VkDevice device,
	VkDescriptorSet descriptor_set,
	uint32_t binding,
	VkImageView view,
	uint32_t offset
);
