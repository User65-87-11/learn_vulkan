#pragma once
#include <vulkan/vulkan_core.h>
#include "util/common.h"
#include "vertex.h"

#define BUFFER_UBO_USAGE VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
#define BUFFER_UBO_PROPS                                                       \
	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

#define BUFFER_SSBO_USAGE                                                      \
	VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
#define BUFFER_SSBO_PROPS                                                      \
	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

#define BUFFER_STAGING_USAGE VK_BUFFER_USAGE_TRANSFER_SRC_BIT
#define BUFFER_STAGING_PROPS                                                   \
	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

#define BUFFER_VERTEX_USAGE                                                    \
	VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
#define BUFFER_VERTEX_PROPS VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

#define BUFFER_INDEX_USAGE                                                     \
	VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
#define BUFFER_INDEX_PROPS VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

#define IMAGE_TEXTURE_USAGE                                                    \
	VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |             \
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT
#define IMAGE_TEXTURE_PROPS VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

struct Buffer {
	VkBuffer handle;
	VkDeviceMemory memory;
	VkDeviceSize size;
	void* mapped;
	VkBufferUsageFlags usage;
};

struct Image {
	VkImage handle;
	VkDeviceMemory memory;
	VkImageView view;
	VkFormat format;
	uint32_t width;
	uint32_t height;
	uint32_t mip_levels;
};

struct Texture {
	struct Image image;
	VkSampler sampler;
};

void Resource_CreateBuffer(VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	struct Buffer* out);

void Resource_AppendToBuffer(
	struct Buffer* dest, uint32_t dest_offset, void* data, uint32_t size);

void Resource_FreeBuffer(struct Buffer* image);

void Resource_CreateImage(uint32_t width,
	uint32_t height,
	uint32_t mip_levels,
	VkFormat format,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties,
	struct Image* out);
void Resource_CreateImageView(
	struct Image* img, VkImageAspectFlagBits aspectFlags);

void Resource_CreateTexture(void* pixel_data,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	struct Texture* out);

void Resource_transitionImageLayout(VkCommandBuffer cmdBuffer,
	VkImage* image,
	VkImageLayout oldLayout,
	VkImageLayout newLayout,

	VkAccessFlags srcAccessMask,
	VkAccessFlags dstAccessMask,

	VkPipelineStageFlags srcStageMask,
	VkPipelineStageFlags dstStageMask,

	VkImageAspectFlagBits aspectFlags,

	uint32_t mipLevels);

VkCommandBuffer Resource_beginSingleTimeCommands();

void Resource_endSingleTimeCommands(VkCommandBuffer commandBuffer);

void Resource_mapBufferMemory(struct Buffer* buffer);

void Resource_unmapBufferMemory(struct Buffer* buffer);

void Resource_FreeImage(struct Image* image);
void Resource_FreeTexture(struct Texture* texture);
