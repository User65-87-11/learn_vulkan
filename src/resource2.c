#include "resource2.h"
#include "resource.h"

void Resource2_CreateTexture(
	struct Device_State * device, 
	void* data,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	struct Image* out) 
{
	PRINT_FNAME;

	if (data == NULL) {
		EXIT_CLEAN("NO IMAGE DATA");
	}
	printf(" w: %d\n",width);
	uint32_t w = width;
	uint32_t h = height;
	out->mip_levels = Resource_getMipmapLevels(device,w, h);

	VkDeviceSize imageSize = w * h * 4;

	struct Buffer staging;
	Resource_CreateBuffer(
		device,
		imageSize, 
		BUFFER_STAGING_USAGE,
	 	BUFFER_STAGING_PROPS, 
		&staging
	);
	Resource_mapBufferMemory(device,&staging);
	memcpy(staging.mapped, data, imageSize);
	Resource_unmapBufferMemory(device,&staging);

	Resource_ImageAllocate(device,w, h, 
		out->mip_levels, 
		format,
		VK_IMAGE_TILING_OPTIMAL, 
		IMAGE_TEXTURE_USAGE, 
		IMAGE_TEXTURE_PROPS,
		out
	);
	VkCommandBuffer command = Device_beginSingleTimeCommands(device);

	Resource_transitionImageLayout(device,command, &out->handle,
		VK_IMAGE_LAYOUT_UNDEFINED, 
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		0,
		VK_ACCESS_2_TRANSFER_WRITE_BIT, 
		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_TRANSFER_BIT, 
		VK_IMAGE_ASPECT_COLOR_BIT,
		out->mip_levels);

	Resouce_copyBufferToImage(device,command, &staging.handle, &out->handle, w, h);
	Resource_generateMipmaps(device,
		command, &out->handle, format, w, h, out->mip_levels);

	Device_endSingleTimeCommands(device,command);

	Resource_FreeBuffer(device,&staging);
	Resource_CreateImageView(device,out, VK_IMAGE_ASPECT_COLOR_BIT);

	// Resouce_createTextureSampler(device,&out->sampler);
}



void Resource2_CreateImageBuffer(
	struct Device_State * device, 
	void* data,
	uint32_t data_size,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	struct Image* out
) {
	PRINT_FNAME;
	if (data == NULL) {
		EXIT_CLEAN("NO IMAGE DATA");
	}
	
	uint32_t w = width;
	uint32_t h = height;
	out->mip_levels = 1;

	VkDeviceSize imageSize = data_size;

	struct Buffer staging;
	Resource_CreateBuffer(
		device,
		imageSize, 
		BUFFER_STAGING_USAGE,
	 	BUFFER_STAGING_PROPS, 
		&staging
	);
	Resource_mapBufferMemory(device,&staging);
	memcpy(staging.mapped, data, imageSize);
	Resource_unmapBufferMemory(device,&staging);

	Resource_ImageAllocate(device,w, h, 
		out->mip_levels, 
		format,
		VK_IMAGE_TILING_OPTIMAL, 
		IMAGE_BUF_IMAGE_USAGE, 
		IMAGE_BUF_IMAGE_PROPS,
		out
	);
	VkCommandBuffer command = Device_beginSingleTimeCommands(device);

	Resource_transitionImageLayout(device,command, &out->handle,
		VK_IMAGE_LAYOUT_UNDEFINED, 
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		0,
		VK_ACCESS_2_TRANSFER_WRITE_BIT, 
		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_TRANSFER_BIT, 
		VK_IMAGE_ASPECT_COLOR_BIT,
		out->mip_levels
	);

	Resouce_copyBufferToImage(device,command, &staging.handle, &out->handle, w, h);

	Resource_transitionImageLayout(device, 
		command, &out->handle,
	    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	    VK_IMAGE_LAYOUT_GENERAL,  // Supports both read AND write
	    VK_ACCESS_2_TRANSFER_WRITE_BIT,
	    VK_ACCESS_2_SHADER_READ_BIT | VK_ACCESS_2_SHADER_WRITE_BIT,
	    VK_PIPELINE_STAGE_2_TRANSFER_BIT,
	    VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
	    VK_IMAGE_ASPECT_COLOR_BIT,
	    out->mip_levels
	);

	Device_endSingleTimeCommands(device,command);

	Resource_FreeBuffer(device,&staging);
	Resource_CreateImageView(device,out, VK_IMAGE_ASPECT_COLOR_BIT);	
}


void Resource2_FreeTexture(struct Device_State * device,struct Image* image) {

	Resource_FreeImage(device,image);

}
