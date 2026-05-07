#include <vulkan/vulkan_core.h>
#include <string.h>
#include <limits.h>
#include "resource.h"
#include "device.h"
#include "util/common.h"
#include "util/gm_array.h"
#include "util/gm_list.h"
#include "shader_common.h"
#include "vertex.h"





static uint8_t  texture0[] = 
{
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7A, 0x7A, 0xF4, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x01, 0xD9, 0xC9, 0x2C, 0x7F, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xB1, 0x8F, 0x0B, 0xFC, 0x61, 0x05, 0x00, 0x00, 0x00, 0x20, 0x63, 0x48, 0x52, 0x4D, 0x00, 0x00, 0x7A, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00, 0xFA, 0x00, 0x00, 0x00, 0x80, 0xE8, 0x00, 0x00, 0x75, 0x30, 0x00, 0x00, 0xEA, 0x60, 0x00, 0x00, 0x3A, 0x98, 0x00, 0x00, 0x17, 0x70, 0x9C, 0xBA, 0x51, 0x3C, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x2E, 0x23, 0x00, 0x00, 0x2E, 0x23, 0x01, 0x78, 0xA5, 0x3F, 0x76, 0x00, 0x00, 0x00, 0x41, 0x49, 0x44, 0x41, 0x54, 0x58, 0xC3, 0xED, 0xD6, 0xC1, 0x09, 0x00, 0x30, 0x0C, 0x42, 0xD1, 0xA4, 0x3B, 0x9A, 0x21, 0x5D, 0xD2, 0x0E, 0x91, 0x43, 0x29, 0x7C, 0xEF, 0xC2, 0x03, 0x2F, 0xB6, 0xA4, 0xD4, 0x22, 0xB6, 0x37, 0xF5, 0x3A, 0xF5, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x4E, 0xB2, 0xFA, 0x03, 0x33, 0xC3, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xDF, 0x80, 0x0B, 0xBB, 0xA7, 0x09, 0x39, 0x81, 0xAB, 0x3D, 0x73, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

static struct Vertex cube0_vertices[] = {
    // +Z (front)
    {{-0.5f,-0.5f, 0.5f}, {0,0,1}, {0,0}},
    {{ 0.5f,-0.5f, 0.5f}, {0,0,1}, {1,0}},
    {{ 0.5f, 0.5f, 0.5f}, {0,0,1}, {1,1}},
    {{-0.5f, 0.5f, 0.5f}, {0,0,1}, {0,1}},
    // -Z (back)
    {{ 0.5f,-0.5f,-0.5f}, {0,0,-1}, {0,0}},
    {{-0.5f,-0.5f,-0.5f}, {0,0,-1}, {1,0}},
    {{-0.5f, 0.5f,-0.5f}, {0,0,-1}, {1,1}},
    {{ 0.5f, 0.5f,-0.5f}, {0,0,-1}, {0,1}},
    // -X (left)
    {{-0.5f,-0.5f,-0.5f}, {-1,0,0}, {0,0}},
    {{-0.5f,-0.5f, 0.5f}, {-1,0,0}, {1,0}},
    {{-0.5f, 0.5f, 0.5f}, {-1,0,0}, {1,1}},
    {{-0.5f, 0.5f,-0.5f}, {-1,0,0}, {0,1}},
    // +X (right)
    {{ 0.5f,-0.5f, 0.5f}, {1,0,0}, {0,0}},
    {{ 0.5f,-0.5f,-0.5f}, {1,0,0}, {1,0}},
    {{ 0.5f, 0.5f,-0.5f}, {1,0,0}, {1,1}},
    {{ 0.5f, 0.5f, 0.5f}, {1,0,0}, {0,1}},
    // +Y (top)
    {{-0.5f, 0.5f, 0.5f}, {0,1,0}, {0,0}},
    {{ 0.5f, 0.5f, 0.5f}, {0,1,0}, {1,0}},
    {{ 0.5f, 0.5f,-0.5f}, {0,1,0}, {1,1}},
    {{-0.5f, 0.5f,-0.5f}, {0,1,0}, {0,1}},
    // -Y (bottom)
    {{-0.5f,-0.5f,-0.5f}, {0,-1,0}, {0,0}},
    {{ 0.5f,-0.5f,-0.5f}, {0,-1,0}, {1,0}},
    {{ 0.5f,-0.5f, 0.5f}, {0,-1,0}, {1,1}},
    {{-0.5f,-0.5f, 0.5f}, {0,-1,0}, {0,1}},
};

static uint32_t cube0_indices[] = {
    // front
    0, 1, 2, 2, 3, 0,
    // back
    4, 5, 6, 6, 7, 4,
    // left
    8, 9,10,10,11, 8,
    // right
    12,13,14,14,15,12,
    // top
    16,17,18,18,19,16,
    // bottom
    20,21,22,22,23,20
};

static struct TextureRes tex_res0;

static VkFence transferFence;

static VkDescriptorImageInfo descriptor_image_info_textures[MAX_TEXTURES] = {};





// static struct GmArray array_vertex_data;
// static struct GmArray array_index_data;

// static struct GmArray array_vertex_ranges;
// static struct GmArray array_index_ranges;


static struct GmList list_BufferRes;

static struct GeometryBuffer global_geometry_buffer;

static struct Buffer ssbo_materials;

static struct GmArray array_meshes;



static VkCommandBuffer transferCommandBuffer;


static VkFence transferFence;


static void createTransferCommandBuffer();

static void CreateImageView(
	struct Image* image,
	VkImageAspectFlagBits aspectFlags
);

static void mapBufferMemory(struct Buffer *buffer) ;

static void unmapBufferMemory(struct Buffer *buffer) ;

static void copyBufferOffset(VkBuffer dstBuffer, uint32_t dstOffset, VkBuffer srcBuffer, uint32_t srcOffset, VkDeviceSize size);

void clearUniformBuffers();

void createShaderDescriptorSetLayout();

void createPickImage();

static void copyBufferOffset(VkBuffer dstBuffer, uint32_t dstOffset, VkBuffer srcBuffer, uint32_t srcOffset,  VkDeviceSize size);
static void copyBuffer(VkBuffer dstBuffer, VkBuffer srcBuffer,  VkDeviceSize size);

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer *buffer,
                  VkDeviceMemory *bufferMemory);

void createTextureImage_from_data(struct TextureRes *tex, uint8_t *in_data,
                                  uint32_t in_size);

void createBufferRes(VkDeviceSize size, VkBufferUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     struct BufferRes *buffer);

void generateMipmaps(VkImage *image, VkFormat imageFormat, int32_t texWidth,
                     int32_t texHeight, uint32_t mipLevels);

void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
                 VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                 VkMemoryPropertyFlags properties, VkImage *image,
                 VkDeviceMemory *imageMemory);

void beginSingleTimeCommands(VkCommandBuffer commandBuffer);

void endSingleTimeCommands(VkCommandBuffer commandBuffer);

void transitionImageLayout(VkCommandBuffer cmdBuffer, VkImage *image,

                           VkImageLayout oldLayout, VkImageLayout newLayout,

                           VkAccessFlags srcAccessMask,
                           VkAccessFlags dstAccessMask,

                           VkPipelineStageFlags srcStageMask,
                           VkPipelineStageFlags dstStageMask,

                           VkImageAspectFlagBits aspectFlags,

                           uint32_t mipLevels);

void copyBufferToImage(VkBuffer *buffer, VkImage *image, uint32_t width,
                       uint32_t height);

void createTextureSamplerShadow(VkSampler *sampler);

void createTextureSampler(VkSampler *sampler);


VkFormat findDepthFormat();

void createDepthResources(struct ImageRes *image);


static void createTransferCommandBuffer(){
		Device_AllocateCommandBuffer(Device_Get()->transfer_pool,&transferCommandBuffer);
}






static void copyBufferOffset(VkBuffer dstBuffer, uint32_t dstOffset, VkBuffer srcBuffer, uint32_t srcOffset, VkDeviceSize size){
	PRINT_FNAME;

	beginSingleTimeCommands(transferCommandBuffer);
	
	// VkBufferCopy bufferCopy = {
	//     .size = size, .dstOffset = 0, .srcOffset = 0
	// };

	VkBufferCopy2 region = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_COPY_2,
		.size = size,
		.dstOffset = dstOffset,
		.srcOffset = srcOffset
	};
	
	VkCopyBufferInfo2 copyBufferInfo2 = {
	    .sType = VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2,
	    .dstBuffer = dstBuffer,
	    .srcBuffer = srcBuffer,
	    .pRegions = &region,
	    .regionCount = 1,
	};
	vkCmdCopyBuffer2(transferCommandBuffer, &copyBufferInfo2);
	
	endSingleTimeCommands(transferCommandBuffer);
}


void Resocue_free(){

	gmArrayFree(&array_meshes);
}

void Resouce_init(){
	

	VkDevice device = Device_Get()->device;
	
	createTransferCommandBuffer();

	gmArrayInit(&array_meshes, sizeof(struct Mesh),MAX_INSTANCES, sizeof(struct Mesh));
	
	VkFenceCreateInfo fenceInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
	};
	vkCreateFence(device, &fenceInfo, NULL, &transferFence);


	Resource_CreateBuffer(
		MAX_VERTICES * sizeof(struct Vertex), 
		BUFFER_VERTEX_USEAGE, 
		BUFFER_VERTEX_PROPS, 
		&global_geometry_buffer.vertex_buffer
	);
	global_geometry_buffer.vertex_capacity = MAX_VERTICES;
	global_geometry_buffer.vertex_used = 0;


	
	Resource_CreateBuffer(
		MAX_INDICES * sizeof(struct Vertex), 
		BUFFER_INDEX_USEAGE, 
		BUFFER_INDEX_PROPS, 
		&global_geometry_buffer.index_buffer
	);
	global_geometry_buffer.index_capacity = MAX_INDICES;
	global_geometry_buffer.index_used = 0;

	
}

void Resource_AppendToBuffer(
	struct Buffer *dest,
	uint32_t dest_offset,
	void * data,
	uint32_t size
){

	struct Buffer staging;
	Resource_CreateBuffer(
		size , 
		BUFFER_STAGING_USEAGE, 
		BUFFER_STAGING_PROPS, 
		&staging
	);
	mapBufferMemory(&staging);
	
	memcpy(staging.mapped, data, size);

	unmapBufferMemory(&staging);

	copyBufferOffset(dest->handle, dest_offset, staging.handle, 0, size);
		
	Resource_FreeBuffer(&staging);
}

void Resource_AppendToVertexBuffer(
	struct Buffer *dest,
	uint32_t dest_vertex_offset,
	struct Vertex * vertices,
	uint32_t vertex_cnt
){
	Resource_AppendToBuffer(
		dest,
		sizeof(struct Vertex) * dest_vertex_offset,
		vertices,
		sizeof(struct Vertex) * vertex_cnt
	);
		
}

void Resource_AppendToIndexBuffer(
	struct Buffer *dest,
	uint32_t dest_index_offset,
	uint32_t * indices,
	uint32_t indices_cnt
){
	Resource_AppendToBuffer(
		dest,
		sizeof(uint32_t) * dest_index_offset,
		indices,
		sizeof(uint32_t) * indices_cnt
	);
}

static void CreateImageView(
	struct Image* image,
	VkImageAspectFlagBits aspectFlags
){
	
	VkDevice device = Device_Get()->device;
  
	VkImageViewCreateInfo viewInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = image->handle,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = image->format,
		.subresourceRange =
		{
			.aspectMask = aspectFlags,
			.baseMipLevel = 0,
			.levelCount = image->mip_levels,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
		
	};
	
	vkCreateImageView(device, &viewInfo, NULL, &image->view);
}
void Resource_CreateImage(
	uint32_t width,
	uint32_t height,
	uint32_t mip_levels,
	VkFormat format,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties,
	struct Image* out
){
	VkDevice device = Device_Get()->device;
	
	VkImageCreateInfo imageCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = format,
		.extent = (VkExtent3D){.width = width, .height = height, .depth = 1},
		.mipLevels = mip_levels,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = tiling,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  vkCreateImage(device, &imageCreateInfo, NULL, &out->handle);

  out->width = width;
  out->height = height;
  out->format = format;
  out->mip_levels = mip_levels;
  
  
  VkImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo2 = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2,
      .image = out->handle,
  };

  VkMemoryRequirements2 memoryRequirements2 = {
      .sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2,
  };

  vkGetImageMemoryRequirements2(
	device, 
	&imageMemoryRequirementsInfo2,
 	&memoryRequirements2
  );

  VkMemoryAllocateInfo allocInfo = {
	.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	.allocationSize = memoryRequirements2.memoryRequirements.size,
	.memoryTypeIndex = Device_findMemoryType(
		memoryRequirements2.memoryRequirements.memoryTypeBits,
		properties
	),

  };

  vkAllocateMemory(device, &allocInfo, NULL, &out->memory);

  vkBindImageMemory(device, out->handle, out->memory, 0);


}

void Resource_FreeImage(
    struct Image* image
){
	VkDevice device = Device_Get()->device;
	if(image->handle!= NULL)
	{
		vkDestroyImage(device, image->handle, NULL);
		image->handle = NULL;
	}
	if(image->view != NULL)
	{
	  	vkDestroyImageView(device, image->view, NULL);
		image->view = NULL;	
	}
	
	if (image->memory != NULL) {
		vkFreeMemory(device, image->memory, NULL);
		image->memory = NULL;
	}
}

void Resource_CreateBuffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    struct Buffer* out
){

	VkDevice device = Device_Get()->device;
	
	VkBufferCreateInfo bufferCreateInfo = {
	
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	
	};

	VkResult res = vkCreateBuffer(device, &bufferCreateInfo, NULL, &out->handle);
	
	if (res != VK_SUCCESS) {
	
		printf(
			"VkDeviceSize :%ld, VkBufferUsageFlags %d, properties %d\n", 
			size,
			usage, 
			properties
		);
		
		EXIT_CLEAN("Cannot create buffer! vkCreateBuffer");
	}

	VkMemoryRequirements memoryRequirements;
	
	vkGetBufferMemoryRequirements(device, out->handle, &memoryRequirements);
	
	VkMemoryAllocateInfo memoryAllocateInfo = {
	
	    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	    .allocationSize = memoryRequirements.size,
	    .memoryTypeIndex = Device_findMemoryType(memoryRequirements.memoryTypeBits, properties),
	
	};

  // VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

  // vkGetPhysicalDeviceMemoryProperties(physicalDevice,
  //                                     &physicalDeviceMemoryProperties);

	vkAllocateMemory(device, &memoryAllocateInfo, NULL, &out->memory);
	
	vkBindBufferMemory(device, out->handle, out->memory, 0);
}

void Resource_FreeBuffer(
    struct Buffer* out
){
	VkDevice device = Device_Get()->device;
	if(out->handle){
	 	vkDestroyBuffer(device, out->handle, NULL);
		out->handle = NULL;
	}
	
	if (out->mapped != NULL) {
		vkUnmapMemory(device, out->memory);
		out->mapped = NULL;
	}
	
	if (out->memory != NULL) {
		vkFreeMemory(device, out->memory, NULL);
		out->memory = NULL;
	}
}

void Resource_free(){


}

void createTextureSamplerShadow(VkSampler *sampler) {
	
	VkPhysicalDevice physicalDevice = getPhysicalDevice();
	VkDevice device = getDevice();
	
	VkPhysicalDeviceProperties physicalDeviceProperties;
	
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	
	VkSamplerCreateInfo samplerInfo = {
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.mipLodBias = 0.0f,
		.minLod = 0.0f,
		.maxLod = VK_LOD_CLAMP_NONE,
		
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		// .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		
		.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy,
		.compareOp = VK_COMPARE_OP_LESS,
		
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};
	
	vkCreateSampler(device, &samplerInfo, NULL, sampler);
}
void createTextureSampler(VkSampler *sampler) {

	VkPhysicalDevice physicalDevice = getPhysicalDevice();
	VkDevice device = getDevice();
	
	VkPhysicalDeviceProperties physicalDeviceProperties;
	
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	
	VkSamplerCreateInfo samplerInfo = {
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.mipLodBias = 0.0f,
		.minLod = 0.0f,
		.maxLod = VK_LOD_CLAMP_NONE,
		
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		
		.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy,
		
		.compareOp = VK_COMPARE_OP_ALWAYS,
		
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
  };

  vkCreateSampler(device, &samplerInfo, NULL, sampler);
}

// void createImageView(
// 	struct ImageRes *image,
// 	VkFormat format,
// 	VkImageAspectFlagBits aspectFlags
// ) 
// {

// 	VkDevice device = _getDevice();
                     
//   /*

//   */
//   image->format = format;

//   VkImageViewCreateInfo viewInfo = {
//       .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
//       .image = image->handle,
//       .viewType = VK_IMAGE_VIEW_TYPE_2D,
//       .format = image->format,
//       .subresourceRange =
//           {

//               .aspectMask = aspectFlags,
//               .baseMipLevel = 0,
//               .levelCount = image->mipLevels,
//               .baseArrayLayer = 0,
//               .layerCount = 1,
//           },

//   };

//   vkCreateImageView(device, &viewInfo, NULL, &image->view);
// }

static void mapBufferMemory(struct Buffer *buffer) {
	vkMapMemory(Device_Get()->device, buffer->memory, 0, buffer->size, 0, &buffer->mapped);
}
static void unmapBufferMemory(struct Buffer *buffer) {

  vkUnmapMemory(Device_Get()->device, buffer->memory);
  buffer->mapped = NULL;
}


void createResources() {
  PRINT_FNAME;
  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

    // struct Frame *frame = &frames[i];

    createBufferRes(sizeof(struct Instance_ssbo) * (MAX_INSTANCES),
                    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    &ssbo_instances[i]);
    mapBufferMemory(&ssbo_instances[i]);
    gmListPushBack(&list_BufferRes, &ssbo_instances[i]);

    createBufferRes(sizeof(struct Global_ubo),
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    &ubo_global[i]);
    mapBufferMemory(&ubo_global[i]);
    gmListPushBack(&list_BufferRes, &ubo_global[i]);
  }

  createBufferRes(sizeof(struct Material_ssbo) * (MAX_INSTANCES),
                  VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                      VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  &ssbo_materials);
  mapBufferMemory(&ssbo_materials);
  gmListPushBack(&list_BufferRes, &ssbo_materials);

  createTextureImage_from_data(&tex_res0, texture0, sizeof(texture0));
  
  createImageView(&tex_res0.image, VK_FORMAT_R8G8B8A8_SRGB,
                  VK_IMAGE_ASPECT_COLOR_BIT);
  createTextureSampler(&tex_res0.textureSampler);

  VkDescriptorImageInfo fallback = {
      .imageView = tex_res0.image.view,
      .sampler = tex_res0.textureSampler,
      .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};

  for (int i = 0; i < MAX_TEXTURES; i++) {
    descriptor_image_info_textures[i] = fallback;
  }
}
void cleanAllocation(struct Allocation *alloc) {

	VkDevice device = getDevice();
	
  if (alloc->mapped != NULL) {

    vkUnmapMemory(device, alloc->memory);
    alloc->mapped = NULL;
  }

  if (alloc->memory != NULL) {
    vkFreeMemory(device, alloc->memory, NULL);
    alloc->memory = NULL;
  }
}
VkFormat findDepthFormat() {

  VkFormat formats[] = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
                        VK_FORMAT_D24_UNORM_S8_UINT};

  return findSupportedFormat(formats, ARR_LEN(formats), VK_IMAGE_TILING_OPTIMAL,
                             VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void createDepthImages() {

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    // struct Frame *frame = &frames[i];

    createDepthResources(&depth[i]);
  }
}
void cleanImageRes(struct ImageRes *img) {

	VkDevice device = getDevice();
  vkDestroyImage(device, img->handle, NULL);
  vkDestroyImageView(device, img->view, NULL);
  cleanAllocation(&img->alloc);
}
void cleanBuffer(struct BufferRes *buff) {
	VkDevice device = getDevice();
  if (buff->handle != NULL) {
    vkDestroyBuffer(device, buff->handle, NULL);
    buff->handle = NULL;
  }

  cleanAllocation(&buff->alloc);
}
void cleanTextureRes(struct TextureRes *tex) {

	VkDevice device = getDevice();
  cleanImageRes(&tex->image);
  vkDestroySampler(device, tex->textureSampler, NULL);
}

void createIndexBuffer2(uint32_t indicesNum, uint32_t *indices,
                        struct BufferRes *out) {

  VkDeviceSize bufferSize = sizeof(uint32_t) * indicesNum;

  struct BufferRes staging = {};

  createBufferRes(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  &staging);

  mapBufferMemory(&staging);

  memcpy(staging.alloc.mapped, indices, staging.alloc.size);

  unmapBufferMemory(&staging);

  createBufferRes(bufferSize,
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                      VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, out);

  copyBuffer(staging.handle, out->handle, bufferSize);

  cleanBuffer(&staging);
}
void createBufferRes(VkDeviceSize size, VkBufferUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     struct BufferRes *buffer) 
{
	VkDevice device = getDevice();                     
  memset(buffer, 0, sizeof(struct BufferRes));
  buffer->alloc.size = size;
  createBuffer(size, usage, properties, &buffer->handle, &buffer->alloc.memory);
}



void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer *buffer,
                  VkDeviceMemory *bufferMemory)
{
	VkDevice device = getDevice();
	VkPhysicalDevice physicalDevice = getPhysicalDevice();

  VkBufferCreateInfo bufferCreateInfo = {

      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .size = size,
      .usage = usage,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,

  };

  VkResult res = vkCreateBuffer(device, &bufferCreateInfo, NULL, buffer);

  if (res != VK_SUCCESS) {

    printf("VkDeviceSize :%ld, VkBufferUsageFlags %d, properties %d\n", size,
           usage, properties);

    EXIT_CLEAN("Cannot create buffer! vkCreateBuffer");
  }

  VkMemoryRequirements memoryRequirements = {

  };

  vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);

  VkMemoryAllocateInfo memoryAllocateInfo = {

      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = memoryRequirements.size,
      .memoryTypeIndex =
          findMemoryType(memoryRequirements.memoryTypeBits, properties),

  };

  VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

  vkGetPhysicalDeviceMemoryProperties(physicalDevice,
                                      &physicalDeviceMemoryProperties);

  vkAllocateMemory(device, &memoryAllocateInfo, NULL, bufferMemory);

  vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
}


void copyBuffer(VkBuffer dstBuffer, VkBuffer srcBuffer,  VkDeviceSize size) {

  PRINT_FNAME;

  beginSingleTimeCommands(transferCommandBuffers);

  VkBufferCopy bufferCopy = {
      .size = size, .dstOffset = 0, .srcOffset = 0};
  VkCopyBufferInfo2 copyBufferInfo2 = {
      .sType = VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2,
      .dstBuffer = dstBuffer,
      .srcBuffer = srcBuffer,
      
      .pRegions =
          &(VkBufferCopy2){
              .sType = VK_STRUCTURE_TYPE_BUFFER_COPY_2,
              .size = size,
              
          },
      .regionCount = 1,
  };
  vkCmdCopyBuffer2(transferCommandBuffers, &copyBufferInfo2);

  endSingleTimeCommands(transferCommandBuffers);
}

void createVertexBuffer2(

    uint32_t verticesNum, void *vertices, uint32_t data_size,
    struct BufferRes *out

) {

  struct BufferRes staging = {};

  createBufferRes(data_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  &staging);

  mapBufferMemory(&staging);

  memcpy(staging.alloc.mapped, vertices, staging.alloc.size);

  unmapBufferMemory(&staging);

  createBufferRes(data_size,
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, out);

  copyBuffer(staging.handle, out->handle, data_size);

  cleanBuffer(&staging);
}


void createMeshBuffers() {
  PRINT_FNAME;
  int vert_num = 0;
  int index_num = 0;
  for (int i = 0; i < array_vertex_ranges.len; i++) {
    struct Range *range = gmArrayGet(&array_vertex_ranges, i);
    vert_num += range->len;
  }
  createVertexBuffer2(vert_num, array_vertex_data.data, array_vertex_data.len,
                      &vertex_buffer);

  for (int i = 0; i < array_index_ranges.len; i++) {
    struct Range *range = gmArrayGet(&array_index_ranges, i);
    index_num += range->len;
  }
  createIndexBuffer2(index_num, array_index_data.data, &index_buffer);
}
void createDepthResources(struct ImageRes *image) {
  PRINT_FNAME;

  VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

  // if(sampled_bit)
  // {
  // 	usage |=VK_IMAGE_USAGE_SAMPLED_BIT;
  // }

  VkFormat depthFormat = findDepthFormat();
  image->alloc.mapped = NULL;
  image->alloc.memory = VK_NULL_HANDLE;
  image->mipLevels = 1;

  createImage(swapChainExtent.width, swapChainExtent.height, 1, depthFormat,
              VK_IMAGE_TILING_OPTIMAL, usage,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &image->handle,
              &image->alloc.memory);

  createImageView(image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

  beginSingleTimeCommands(transferCommandBuffers);

  transitionImageLayout(
      transferCommandBuffers, &image->handle, VK_IMAGE_LAYOUT_UNDEFINED,

      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 0,
      VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

  endSingleTimeCommands(transferCommandBuffers);
}


void copyBufferToImage(VkBuffer *buffer, VkImage *image, uint32_t width,
                       uint32_t height) {
  PRINT_FNAME;

  beginSingleTimeCommands(transferCommandBuffers);

  VkBufferImageCopy bufferImageCopy = {
      .bufferOffset = 0,
      .bufferRowLength = 0,
      .bufferImageHeight = 0,
      .imageSubresource =
          {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              0,
              0,
              1,
          },
      .imageOffset = {0, 0, 0},
      .imageExtent = {width, height, 1},
  };

  vkCmdCopyBufferToImage(transferCommandBuffers, *buffer, *image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
                         &bufferImageCopy);

  endSingleTimeCommands(transferCommandBuffers);
}
void transitionImageLayout(VkCommandBuffer cmdBuffer, VkImage *image,

                           VkImageLayout oldLayout, VkImageLayout newLayout,

                           VkAccessFlags srcAccessMask,
                           VkAccessFlags dstAccessMask,

                           VkPipelineStageFlags srcStageMask,
                           VkPipelineStageFlags dstStageMask,

                           VkImageAspectFlagBits aspectFlags,
                           uint32_t mipLevels) {

  VkImageMemoryBarrier2 imageMemoryBarrier2 = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

  };

  VkImageMemoryBarrier imageMemoryBarriers = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .oldLayout = oldLayout,
      .newLayout = newLayout,

      .image = *image,
      .subresourceRange = {.aspectMask = aspectFlags,
                           .baseMipLevel = 0,
                           .levelCount = mipLevels,
                           .baseArrayLayer = 0,
                           .layerCount = 1},
      .srcAccessMask = srcAccessMask,
      .dstAccessMask = dstAccessMask,
  };

  vkCmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, 0, 0,
                       (VkMemoryBarrier *)NULL, 0,
                       (VkBufferMemoryBarrier *)NULL, 1, &imageMemoryBarriers);
}

static void beginSingleTimeCommands(VkCommandBuffer commandBuffer) {

  vkResetCommandBuffer(commandBuffer, 0);

  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  vkBeginCommandBuffer(commandBuffer, &beginInfo);
}
static void endSingleTimeCommands(VkCommandBuffer commandBuffer) {

	VkDevice device = getDevice();
	
	vkEndCommandBuffer(commandBuffer);
	
	VkPipelineStageFlags2 stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
	
	VkSubmitInfo2 submitInfo2 = {
	    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
	    .commandBufferInfoCount = 1,
	    .pCommandBufferInfos =
	        &(VkCommandBufferSubmitInfo){
	            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
	            .commandBuffer = commandBuffer,
	
	        },
	
	};
	
	vkResetFences(device, 1, &transferFence);
	
	vkQueueSubmit2(transferQueue, 1, &submitInfo2, transferFence);
	
	vkWaitForFences(device, 1, &transferFence, VK_TRUE, UINT64_MAX);
}

void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
                 VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                 VkMemoryPropertyFlags properties, VkImage *image,
                 VkDeviceMemory *imageMemory) {

  VkImageCreateInfo imageCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .imageType = VK_IMAGE_TYPE_2D,
      .format = format,
      .extent = (VkExtent3D){.width = width, .height = height, .depth = 1},
      .mipLevels = mipLevels,
      .arrayLayers = 1,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .tiling = tiling,
      .usage = usage,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  vkCreateImage(device, &imageCreateInfo, NULL, image);

  VkImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo2 = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2,
      .image = *image,
  };

  VkMemoryRequirements2 memoryRequirements2 = {
      .sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2,
  };

  vkGetImageMemoryRequirements2(device, &imageMemoryRequirementsInfo2,
                                &memoryRequirements2);

  VkMemoryAllocateInfo allocInfo = {
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = memoryRequirements2.memoryRequirements.size,
      .memoryTypeIndex = findMemoryType(
          memoryRequirements2.memoryRequirements.memoryTypeBits, properties),

  };

  vkAllocateMemory(device, &allocInfo, NULL, imageMemory);

  vkBindImageMemory(device, *image, *imageMemory, 0);
}

uint32_t getMipmapLevels(uint32_t w, uint32_t h) {
  uint32_t max = w > h ? w : h;
  uint32_t d = floor(log2(max));
  d++;
  return d;
}

void createTextureImage_from_data(struct TextureRes *tex, uint8_t *in_data,
                                  uint32_t in_size) {
  PRINT_FNAME;

  printf("data:%p, size:%d\n", in_data, in_size);
  int texWidth, texHeight, texChannels;

  stbi_uc *pixels = stbi_load_from_memory(
      in_data, in_size, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

  if (!pixels) {
    EXIT_CLEAN("failed to load texture image!");
  }

  printf("w:%d, h:%d c:%d\n", texWidth, texHeight, texChannels);

  tex->image.mipLevels = getMipmapLevels(texWidth, texHeight);

  tex->image.alloc.mapped = NULL;
  tex->image.alloc.memory = VK_NULL_HANDLE;

  VkDeviceSize imageSize = texWidth * texHeight * 4;

  VkBuffer stagingBuffer;

  VkDeviceMemory stagingBufferMemory;

  createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               &stagingBuffer, &stagingBufferMemory);

  void *data = NULL;

  vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
  memcpy(data, pixels, imageSize);

  vkUnmapMemory(device, stagingBufferMemory);

  stbi_image_free(pixels);

  createImage(texWidth, texHeight, tex->image.mipLevels,
              VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &tex->image.handle,
              &tex->image.alloc.memory);

  beginSingleTimeCommands(transferCommandBuffers);

  transitionImageLayout(
      transferCommandBuffers, &tex->image.handle, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_2_TRANSFER_WRITE_BIT,
      VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_TRANSFER_BIT,
      VK_IMAGE_ASPECT_COLOR_BIT, tex->image.mipLevels);

  endSingleTimeCommands(transferCommandBuffers);

  copyBufferToImage(&stagingBuffer, &tex->image.handle, texWidth, texHeight);

  vkDestroyBuffer(device, stagingBuffer, NULL);

  vkFreeMemory(device, stagingBufferMemory, NULL);

  generateMipmaps(&tex->image.handle, VK_FORMAT_R8G8B8A8_SRGB, texWidth,
                  texHeight, tex->image.mipLevels);
}

void createTextureImage(struct TextureRes *tex, char *path

) {

  PRINT_FNAME;

  int texWidth, texHeight, texChannels;

  stbi_uc *pixels =
      stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

  tex->image.mipLevels = getMipmapLevels(texWidth, texHeight);

  tex->image.alloc.mapped = NULL;
  tex->image.alloc.memory = VK_NULL_HANDLE;

  VkDeviceSize imageSize = texWidth * texHeight * 4;

  if (!pixels) {
    EXIT_CLEAN("failed to load texture image!");
  }

  VkBuffer stagingBuffer;

  VkDeviceMemory stagingBufferMemory;

  createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               &stagingBuffer, &stagingBufferMemory);

  void *data = NULL;

  vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
  memcpy(data, pixels, imageSize);

  vkUnmapMemory(device, stagingBufferMemory);

  stbi_image_free(pixels);

  createImage(texWidth, texHeight, tex->image.mipLevels,
              VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &tex->image.handle,
              &tex->image.alloc.memory);

  beginSingleTimeCommands(transferCommandBuffers);

  transitionImageLayout(
      transferCommandBuffers, &tex->image.handle, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_2_TRANSFER_WRITE_BIT,
      VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_TRANSFER_BIT,
      VK_IMAGE_ASPECT_COLOR_BIT, tex->image.mipLevels);

  endSingleTimeCommands(transferCommandBuffers);

  copyBufferToImage(&stagingBuffer, &tex->image.handle, texWidth, texHeight);

  vkDestroyBuffer(device, stagingBuffer, NULL);

  vkFreeMemory(device, stagingBufferMemory, NULL);

  generateMipmaps(&tex->image.handle, VK_FORMAT_R8G8B8A8_SRGB, texWidth,
                  texHeight, tex->image.mipLevels);
}

void generateMipmaps(VkImage *image, VkFormat imageFormat, int32_t texWidth,
                     int32_t texHeight, uint32_t mipLevels) {

  PRINT_FNAME;

  beginSingleTimeCommands(transferCommandBuffers);

  VkFormatProperties2 formatProperties = {
      .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2,
  };
  vkGetPhysicalDeviceFormatProperties2(physicalDevice, imageFormat,
                                       &formatProperties);

  if (!(formatProperties.formatProperties.optimalTilingFeatures &
        VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
    EXIT_CLEAN("texture image format does not support linear blitting!");
  }

  int32_t mipWidth = texWidth;
  int32_t mipHeight = texHeight;

  VkImageMemoryBarrier2 barrier = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = *image,
      .subresourceRange =
          {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1,
          },
      .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      .srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT,
      .srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
  };

  VkDependencyInfo dependencyInfo = {
      .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &barrier,
  };

  vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);

  for (uint32_t i = 1; i < mipLevels; i++) {

    VkImageBlit2 blit = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
        .srcOffsets = {{0, 0, 0}, {mipWidth, mipHeight, 1}},
        .dstOffsets = {{0, 0, 0},
                       {mipWidth > 1 ? mipWidth / 2 : 1,
                        mipHeight > 1 ? mipHeight / 2 : 1, 1}},
        .srcSubresource =
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = i - 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
        .dstSubresource =
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = i,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
    };

    VkBlitImageInfo2 blitInfo = {
        .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2,
        .srcImage = *image,
        .srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        .dstImage = *image,
        .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .filter = VK_FILTER_LINEAR,
        .regionCount = 1,
        .pRegions = &blit,
    };

    vkCmdBlitImage2(transferCommandBuffers, &blitInfo);

    barrier.subresourceRange.baseMipLevel = i - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
    barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;

    vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);

    if (i < mipLevels - 1) {
      barrier.subresourceRange.baseMipLevel = i;
      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
      barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
      barrier.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;

      vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);
    }

    if (mipWidth > 1)
      mipWidth /= 2;
    if (mipHeight > 1)
      mipHeight /= 2;
  }

  barrier.subresourceRange.baseMipLevel = mipLevels - 1;
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
  barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
  barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;

  vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);

  endSingleTimeCommands(transferCommandBuffers);
}



