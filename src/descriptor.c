#include <string.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>

#include "descriptor.h"
#include "device.h"
#include "resource.h"
#include "shader_common.h"
#include "common.h"




void Descriptor_UpdateBuffer(
	VkDevice device,
	VkDescriptorSet set,
	uint32_t binding,
	VkDescriptorType type,
	VkBuffer buffer,
	VkDeviceSize size,
	uint32_t offset
	
) {
	// Update Global descriptor set (UBO)
	VkDescriptorBufferInfo globalBufferInfo = {
		.buffer = buffer, .offset = offset, .range = size};

	VkWriteDescriptorSet wite = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = set,
		.dstBinding = binding,
		.dstArrayElement = 0,
		.descriptorType = type,
		.descriptorCount = 1,
		.pBufferInfo = &globalBufferInfo};

	vkUpdateDescriptorSets(device, 1, &wite, 0, NULL);
}

void Descriptor_UpdateTextureDescriptors(
	VkDevice device,
	VkDescriptorSet descriptor_set,
	uint32_t binding,
	VkImageView view,
	uint32_t offset) {
	PRINT_FNAME;


	VkDescriptorImageInfo info = {
		.imageView = view,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	};

	VkWriteDescriptorSet write = {
		.dstBinding = binding,
		.dstArrayElement = offset,
		.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = descriptor_set,
		.descriptorCount = 1,

		.pImageInfo = &info,
	};

	
	vkUpdateDescriptorSets(
		device, 1, &write, 0, NULL
	);
}

void Descriptor_UpdateBufferImageDescriptors(
	VkDevice device,
	VkDescriptorSet descriptor_set,
	uint32_t binding,
	VkImageView view,
	uint32_t offset
) {
	PRINT_FNAME;


	VkDescriptorImageInfo info = {
		.imageView = view,
		.imageLayout = VK_IMAGE_LAYOUT_GENERAL
	};

	VkWriteDescriptorSet write = {
		.dstBinding = binding,
		.dstArrayElement = offset,
		.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = descriptor_set,
		.descriptorCount = 1,

		.pImageInfo = &info,
	};

	
	vkUpdateDescriptorSets(
		device, 1, &write, 0, NULL
	);
}

void Descriptor_UpdateSamplerDescriptors(
	VkDevice device,
	VkDescriptorSet descriptor_set,
	uint32_t binding,
	VkSampler sampler) {
	PRINT_FNAME;


	VkDescriptorImageInfo info = {
		.sampler = sampler,
		// .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	};
	
	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = descriptor_set;
	write.dstBinding = binding;
	write.dstArrayElement = 0;
	write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	write.descriptorCount = 1;

	write.pImageInfo = &info;

	vkUpdateDescriptorSets(
	device, 1, &write, 0, NULL);
}


void Descriptor_Allocate(
	VkDevice device, 
	VkDescriptorSetLayout layout,
	VkDescriptorPool pool,
	VkDescriptorSet * set,
	uint32_t count
) {

	VkDescriptorSetAllocateInfo allocInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = pool,
		.descriptorSetCount = count,
		.pSetLayouts = &layout
	};
	VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, set));
}
