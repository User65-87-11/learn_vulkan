
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






// static void mapBufferMemory(struct Buffer *buffer) ;

// static void unmapBufferMemory(struct Buffer *buffer) ;

static void copyBufferOffset(VkBuffer dstBuffer, uint32_t dstOffset, VkBuffer srcBuffer, uint32_t srcOffset, VkDeviceSize size);

static void generateMipmaps(
	VkCommandBuffer command,
	VkImage *image, 
	VkFormat imageFormat, 
	int32_t texWidth,
	int32_t texHeight, 
	uint32_t mipLevels
);



static void copyBufferToImage(
	VkCommandBuffer command,
	VkBuffer *buffer, 
	VkImage *image, 
	uint32_t width,
	uint32_t height
);
static void createTextureSampler(VkSampler *sampler);

static uint32_t getMipmapLevels(uint32_t w, uint32_t h);



static void copyBufferOffset(VkBuffer dstBuffer, uint32_t dstOffset, VkBuffer srcBuffer, uint32_t srcOffset, VkDeviceSize size){
	PRINT_FNAME;

	struct Device * device = Device_Get();
	
	VkCommandBuffer command = Resource_beginSingleTimeCommands();


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
	vkCmdCopyBuffer2(command, &copyBufferInfo2);
	
	Resource_endSingleTimeCommands(command);
}




void Resource_AppendToBuffer(
	struct Buffer *dest,
	uint32_t dest_offset,
	void * data,
	uint32_t size
){
	PRINT_FNAME;
	printf("dest->size:%d, size:%d, dest_offset:%d\n",dest->size,size,dest_offset);
	assert(dest->size  >= size + dest_offset);
	
	struct Buffer staging;
	staging.size = size;
	Resource_CreateBuffer(
		size , 
		BUFFER_STAGING_USAGE, 
		BUFFER_STAGING_PROPS, 
		&staging
	);
	Resource_mapBufferMemory(&staging);
	
	memcpy(staging.mapped, data, size);

	Resource_unmapBufferMemory(&staging);

	copyBufferOffset(dest->handle, dest_offset, staging.handle, 0, size);
		
	Resource_FreeBuffer(&staging);


}

void Resource_CreateImageView(
	struct Image* image,
	VkImageAspectFlagBits aspectFlags
){
	PRINT_FNAME;
	
	VkDevice device = Device_Get()->logical_device;
  
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
	VkDevice device = Device_Get()->logical_device;
	
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
	VkDevice device = Device_Get()->logical_device;
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
void Resource_FreeTexture(
    struct Texture* texture
){
	
		
	Resource_FreeImage(&texture->image);
	
	
	if(texture->sampler!= NULL)
	{
	
		
		vkDestroySampler(Device_Get()->logical_device, texture->sampler,NULL);
		texture->sampler = NULL;
	}
	
}

void Resource_CreateBuffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    struct Buffer* out
){

	out->usage = usage;
	out->size = size;
	
	VkDevice device = Device_Get()->logical_device;
	
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
	vkAllocateMemory(device, &memoryAllocateInfo, NULL, &out->memory);
	
	vkBindBufferMemory(device, out->handle, out->memory, 0);


}

static uint32_t getMipmapLevels(uint32_t w, uint32_t h) {
  uint32_t max = w > h ? w : h;
  uint32_t d = floor(log2(max));
  d++;
  return d;
}

void Resource_FreeBuffer(
    struct Buffer* out
){
	VkDevice device = Device_Get()->logical_device;
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

void createTextureSampler(VkSampler *sampler) {
	PRINT_FNAME;
	struct Device * device = Device_Get();
	// VkPhysicalDevice physicalDevice = getPhysicalDevice();
	// VkDevice device = getDevice();
	
	VkPhysicalDeviceProperties physicalDeviceProperties;
	
	vkGetPhysicalDeviceProperties(device->physical_device, &physicalDeviceProperties);
	
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
		
		.compareOp = VK_COMPARE_OP_LESS,
		
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
 	 };

  vkCreateSampler(device->logical_device, &samplerInfo, NULL, sampler);
}
                     
 void Resource_mapBufferMemory(struct Buffer *buffer) {
	vkMapMemory(Device_Get()->logical_device, buffer->memory, 0, buffer->size, 0, &buffer->mapped);
}
 void Resource_unmapBufferMemory(struct Buffer *buffer) {

  vkUnmapMemory(Device_Get()->logical_device, buffer->memory);
  buffer->mapped = NULL;
}

void Resource_CreateTexture(
	void* data,          
    uint32_t width,
    uint32_t height,
    VkFormat format, 
    
	struct Texture *out
){

	if(data == NULL)
	{
		EXIT_CLEAN("NO IMAGE DATA");
	}

	struct Device *device = Device_Get();
	uint32_t w =width;
	uint32_t h = height;
	out->image.mip_levels = getMipmapLevels(w, h);
	
	VkDeviceSize imageSize = w * h * 4;

	struct Buffer staging;
	Resource_CreateBuffer(
		imageSize, 
		BUFFER_STAGING_USAGE, 
		BUFFER_STAGING_PROPS, 
		&staging
	);
	Resource_mapBufferMemory(&staging);
	memcpy(staging.mapped, data, imageSize);
	Resource_unmapBufferMemory(&staging);

	Resource_CreateImage(
		w, 
		h, 
		out->image.mip_levels, 
		format, 
		VK_IMAGE_TILING_OPTIMAL, 
		IMAGE_TEXTURE_USAGE, 
		IMAGE_TEXTURE_PROPS,
		&out->image
	);
	VkCommandBuffer command = Resource_beginSingleTimeCommands();
	
	Resource_transitionImageLayout(
		command,
		&out->image.handle, 
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		0, 
		VK_ACCESS_2_TRANSFER_WRITE_BIT,
		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT, 
		VK_PIPELINE_STAGE_2_TRANSFER_BIT,
		VK_IMAGE_ASPECT_COLOR_BIT, 
		out->image.mip_levels
	);


	copyBufferToImage(
		command,
		&staging.handle, 
		&out->image.handle, w,h
	);
	generateMipmaps(
		command,
		&out->image.handle, 
		format, 
		w, h, 
		out->image.mip_levels
	);

	Resource_endSingleTimeCommands(command);
	
	Resource_FreeBuffer(&staging);
 	Resource_CreateImageView(
  		&out->image,
		VK_IMAGE_ASPECT_COLOR_BIT
	);
  
	createTextureSampler(&out->sampler);
	
}
static void copyBufferToImage(
	VkCommandBuffer command,
	VkBuffer *buffer, 
	VkImage *image, 
	uint32_t width,
	uint32_t height
) {
	PRINT_FNAME;


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

	vkCmdCopyBufferToImage(
		command, 
		*buffer, 
		*image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&bufferImageCopy
	);

  // Resource_endSingleTimeCommands(device->transfer_cmd_buffer);
}
void Resource_transitionImageLayout(
	VkCommandBuffer cmdBuffer, 
	VkImage *image,
	VkImageLayout oldLayout, 
	VkImageLayout newLayout,

	VkAccessFlags srcAccessMask,
	VkAccessFlags dstAccessMask,
	
	VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask,

    VkImageAspectFlagBits aspectFlags,
    uint32_t mipLevels
) {

	// // beginSingleTimeCommands(&cmdBuffer);
	
	// VkImageMemoryBarrier2 imageMemoryBarrier2 = {
	//     .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
	
	// };
	
	// VkImageMemoryBarrier imageMemoryBarriers = {
	//     .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
	//     .oldLayout = oldLayout,
	//     .newLayout = newLayout,
	
	//     .image = *image,
	//     .subresourceRange = {.aspectMask = aspectFlags,
	//                         .baseMipLevel = 0,
	//                         .levelCount = mipLevels,
	//                         .baseArrayLayer = 0,
	//                         .layerCount = 1},
	//     .srcAccessMask = srcAccessMask,
	//     .dstAccessMask = dstAccessMask,
	// };
	
	// vkCmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, 0, 0,
	//                     (VkMemoryBarrier *)NULL, 0,
	//                     (VkBufferMemoryBarrier *)NULL, 1, &imageMemoryBarriers);

	{
		VkImageMemoryBarrier2 beginBarrier= {

			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.srcStageMask = srcStageMask,
			.dstStageMask = dstStageMask,
			.srcAccessMask = srcAccessMask,
			.dstAccessMask = dstAccessMask,
			.oldLayout = oldLayout,
			.newLayout = newLayout,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = *image,
			.subresourceRange = {
				
				.aspectMask = aspectFlags,
				.baseMipLevel = 0,
				.levelCount = mipLevels,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
			
			
		};

		VkDependencyInfo beginDepInfo = {
		
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &beginBarrier,
		};
		
		vkCmdPipelineBarrier2(cmdBuffer, &beginDepInfo);
	}
	// endSingleTimeCommands(cmdBuffer);
}

VkCommandBuffer Resource_beginSingleTimeCommands() {

	VkCommandBuffer commandBuffer = Device_createTransferCommandBuffer();
	

	
  // vkResetCommandBuffer(commandBuffer, 0);

  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  return commandBuffer;
}
void Resource_endSingleTimeCommands(VkCommandBuffer commandBuffer) {

	struct Device * device = Device_Get();
	// VkDevice device = Device_Get()->device;
	
	vkEndCommandBuffer(commandBuffer);
	
	// VkPipelineStageFlags2 stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;

	VkCommandBufferSubmitInfo buffersubmitInfo={
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = commandBuffer
	};

	VkSubmitInfo2 submitInfo2 = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &buffersubmitInfo
	
	};
	
	vkResetFences(device->logical_device, 1, &device->transfer_fence);
	
	vkQueueSubmit2(device->transfer_queue, 1, &submitInfo2, device->transfer_fence);
	
	vkWaitForFences(device->logical_device, 1, &device->transfer_fence, VK_TRUE, UINT64_MAX);


	vkFreeCommandBuffers(
        Device_Get()->logical_device,
        Device_Get()->transfer_pool,
        1,
        &commandBuffer
    );
}


	
static void generateMipmaps(
	VkCommandBuffer command,
	VkImage *image, 
	VkFormat imageFormat, 
	int32_t texWidth,
	int32_t texHeight, 
	uint32_t mipLevels
) {

	// struct Device * device = Device_Get();
	PRINT_FNAME;
	
	
	// Resource_beginSingleTimeCommands(device->transfer_cmd_buffer);

	
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
	
	vkCmdPipelineBarrier2(command, &dependencyInfo);
	
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
	
	vkCmdBlitImage2(command, &blitInfo);
	
	barrier.subresourceRange.baseMipLevel = i - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
	barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
	barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
	
	vkCmdPipelineBarrier2(command, &dependencyInfo);
	
	if (i < mipLevels - 1) {
	    barrier.subresourceRange.baseMipLevel = i;
	    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	    barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
	    barrier.dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
	    barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
	    barrier.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
	
	    vkCmdPipelineBarrier2(command, &dependencyInfo);
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
	
	vkCmdPipelineBarrier2(command, &dependencyInfo);
	
	// Resource_endSingleTimeCommands(device->transfer_cmd_buffer);
}
