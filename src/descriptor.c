#include <string.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>

#include "descriptor.h"
#include "device2.h"
#include "shader_common.h"
#include "util/common.h"



// // layouts access
// struct DescriptorContext* Descriptor_GetContext(
// 	struct Device_State* device_ref) {
// 	return &context;
// }

// static void createDescriptors(VkDevice device);

// lifecycle
void Descriptor_Init(struct Descriptor_InitInfo *info, struct DescriptorContext * context) {
	PRINT_FNAME;
	memset(context, 0, sizeof(*context));
	context->device_ref = info->device_ref;

	uint32_t cnt_glob = MAX_FRAMES_IN_FLIGHT * 3;
	uint32_t cnt_inst = MAX_FRAMES_IN_FLIGHT;
	uint32_t cnt_material = 1;
	uint32_t cnt_textures = MAX_TEXTURES;
	uint32_t max_sets = cnt_inst + cnt_glob + cnt_material + cnt_textures;

	VkDevice device = context->device_ref->logical_device;

	VkDescriptorPoolSize poolSizes[] = {

		(VkDescriptorPoolSize){
			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = MAX_TEXTURES},
		(VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = MAX_FRAMES_IN_FLIGHT * 3},
		(VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount =
				MAX_FRAMES_IN_FLIGHT * MAX_INSTANCES + MAX_MATERIALS},

	};

	VkDescriptorPoolCreateInfo poolInfo = {

		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.pNext = NULL,
		// .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		.maxSets = max_sets,
		.poolSizeCount = ARR_LEN(poolSizes),
		.pPoolSizes = poolSizes};

	VkResult result =
		vkCreateDescriptorPool(device, &poolInfo, NULL, &context->pool);
	{
		// VkDescriptorSetLayoutBinding globalBinding = {
		// 	.binding = 0,
		// 	.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		// 	.descriptorCount = 1,
		// 	.stageFlags = VK_SHADER_STAGE_VERTEX_BIT |
		// VK_SHADER_STAGE_FRAGMENT_BIT
		// };

		VkDescriptorSetLayoutBinding bindings[3] = {
			{.binding = BINDING_GLOBAL_GLOBAL,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags =
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},

			{.binding = BINDING_GLOBAL_CAMERA,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags =
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},

			{.binding = BINDING_GLOBAL_LIGHT,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags =
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT}};

		VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = ARR_LEN(bindings),
			.pBindings = bindings};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &globalLayoutInfo, NULL, &context->globalLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN("vkCreateDescriptorSetLayout4 failed");
		}
	}

	{
		VkDescriptorSetLayoutBinding materialBinding = {.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount = 1,
			.stageFlags =
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutCreateInfo materialLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &materialBinding};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &materialLayoutInfo, NULL, &context->materialLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN(
				"vkCreateDescriptorSetLayout  materialDescriptorSetLayout");
		}
	}

	{
		VkDescriptorSetLayoutBinding instanceBindings = {

			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount = 1,
			.stageFlags =
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutCreateInfo instanceLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &instanceBindings};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &instanceLayoutInfo, NULL, &context->instanceLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN(
				"vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
		}
	}

	{
		VkDescriptorSetLayoutBinding samplerBindings = {

			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = MAX_TEXTURES,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT

		};

		VkDescriptorSetLayoutCreateInfo samplerLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &samplerBindings};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &samplerLayoutInfo, NULL, &context->samplerLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN("vkCreateDescriptorSetLayout samplerBindings");
		}
	}
}

void Descriptor_UpdateBuffer(struct DescriptorContext * context,
	VkDescriptorSet set,
	uint32_t binding,
	VkDescriptorType type,
	VkBuffer buffer,
	VkDeviceSize size) {
	// Update Global descriptor set (UBO)
	VkDescriptorBufferInfo globalBufferInfo = {
		.buffer = buffer, .offset = 0, .range = size};

	VkWriteDescriptorSet wite = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = set,
		.dstBinding = binding,
		.dstArrayElement = 0,
		.descriptorType = type,
		.descriptorCount = 1,
		.pBufferInfo = &globalBufferInfo};

	vkUpdateDescriptorSets(context->device_ref->logical_device, 1, &wite, 0, NULL);
}

void Descriptor_SetTextureToDescriptorInfoArray(
	struct DescriptorContext* context,
	VkImageView view,
	VkSampler sampler,
	uint32_t position) {

	PRINT_FNAME;
	VkDescriptorImageInfo info = {.imageView = view,
		.sampler = sampler,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	context->descriptor_image_info_textures[position] = info;
}

void Descriptor_UpdateTextureDescriptors(struct DescriptorContext* context,
	VkDescriptorSet descriptor_set,
	VkDescriptorImageInfo* arr,
	uint32_t count,
	uint32_t offset) {
	PRINT_FNAME;

	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = descriptor_set;
	write.dstBinding = 0;
	write.dstArrayElement = offset;
	write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write.descriptorCount = count;

	write.pImageInfo = arr;

	vkUpdateDescriptorSets(
		context->device_ref->logical_device, 1, &write, 0, NULL);
}

void Descriptor_Destroy(struct DescriptorContext* context) {
	VkDevice device = context->device_ref->logical_device;

	vkDestroyDescriptorPool(device, context->pool, NULL);

	vkDestroyDescriptorSetLayout(device, context->globalLayout, NULL);
	vkDestroyDescriptorSetLayout(device, context->instanceLayout, NULL);
	vkDestroyDescriptorSetLayout(device, context->materialLayout, NULL);
	vkDestroyDescriptorSetLayout(device, context->samplerLayout, NULL);
}

// pool control
void Descriptor_ResetPool(struct DescriptorContext* context) {

	vkResetDescriptorPool(context->device_ref->logical_device, context->pool, 0);
}

VkDescriptorSet Descriptor_Allocate(
	struct DescriptorContext* context, VkDescriptorSetLayout layout) {

	VkDescriptorSetAllocateInfo allocInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = context->pool,
		.descriptorSetCount = 1,
		.pSetLayouts = &layout};

	VkDescriptorSet set;
	vkAllocateDescriptorSets(context->device_ref->logical_device, &allocInfo, &set);
	return set;
}

// updates
// void Descriptor_UpdateBuffer(struct DescriptorContext* context,
// 	VkDescriptorSet set,
// 	uint32_t binding,
// 	VkDescriptorType type,
// 	VkBuffer buffer,
// 	VkDeviceSize size);

// void Descriptor_UpdateImage(struct DescriptorContext* context,
// 	VkDescriptorSet set,
// 	uint32_t binding,
// 	VkDescriptorImageInfo* imageInfo);
