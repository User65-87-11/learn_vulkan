#pragma once
#include <vulkan/vulkan_core.h>

#include "device.h"
#include "common.h"
#include "vertex.h"
#include "resource.h"


void Resource2_CreateTexture(
	struct Device_State* device,
	void* pixel_data,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	struct Image* out
);


void Resource2_FreeTexture(
	struct Device_State * device,
	struct Image* texture);


void Resource2_CreateImageBuffer(
	struct Device_State * device, 
	void* data,
	uint32_t data_size,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	struct Image* out
);
