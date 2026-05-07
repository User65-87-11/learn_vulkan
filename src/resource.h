#pragma once
#include <vulkan/vulkan_core.h>
#include "util/common.h"
#include "vertex.h"

#define BUFFER_UBO_USEAGE VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
#define BUFFER_UBO_PROPS VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

#define BUFFER_SSBO_USEAGE VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
#define BUFFER_SSBO_PROPS VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

#define BUFFER_STAGING_USEAGE VK_BUFFER_USAGE_TRANSFER_SRC_BIT
#define BUFFER_STAGING_PROPS VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

#define BUFFER_VERTEX_USEAGE  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
#define BUFFER_VERTEX_PROPS  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

#define BUFFER_INDEX_USEAGE VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
#define BUFFER_INDEX_PROPS  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT





struct Buffer {
    VkBuffer handle;
    VkDeviceMemory memory;
    VkDeviceSize size;
    void* mapped;
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

struct GeometryBuffer {
	struct Buffer vertex_buffer;
	struct Buffer index_buffer;

	uint32_t vertex_used;
    uint32_t index_used;
    
    uint32_t vertex_capacity;
    uint32_t index_capacity;

};

struct Mesh{
	struct Range range_vertex;
	struct Range range_index;
};


void Resource_CreateBuffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    struct Buffer* out
);

void Resource_AppendToVertexBuffer(
	struct Buffer *dest,
	uint32_t dest_vertex_offset,
	struct Vertex * vertices,
	uint32_t vertex_cnt
);

void Resource_AppendToIndexBuffer(
	struct Buffer *dest,
	uint32_t dest_index_offset,
	uint32_t * indices,
	uint32_t indices_cnt
);
void Resource_AppendToBuffer(
	struct Buffer *dest,
	uint32_t dest_offset,
	void * data,
	uint32_t size
);

void Resource_FreeBuffer(
    struct Buffer* image
);

void Resource_CreateImage(
    uint32_t width,
    uint32_t height,
    uint32_t mip_levels,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    struct Image* out
);

void Resource_FreeImage(
    struct Image* image
);



void Resouce_init();

void Resocue_free();