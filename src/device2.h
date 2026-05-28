#pragma once
#include "src/instance.h"
#include <vulkan/vulkan_core.h>


struct Device_Queue{
   	VkQueue queue;
   	uint32_t index;
};

struct Device_State {
	struct Instance_State * ref_instance;
	struct Platform_State * ref_platform;
	
	VkPhysicalDevice physical_device;
	VkDevice logical_device;

	struct Device_Queue queue_graphics;
	struct Device_Queue queue_tranfser;
	struct Device_Queue queue_present;



	
	VkCommandPool command_pool_graphics;
	VkCommandPool command_pool_transfer;
	// VkCommandBuffer transfer_cmd_buffer;
	VkFence transfer_fence;
};

struct Device_CreateInfo{
	//VkInstance  instance; 
	//VkSurfaceKHR surface;
	struct Platform_State * ref_platform;
	struct Instance_State * ref_instance;
	
	const char ** requiredDeviceExtensions;
	uint32_t requiredDeviceExtensions_cnt;
};

void Device_Create(struct Device_CreateInfo * info, struct Device_State * device);

void Device_Destroy(struct Device_State * device);

// void Device_WaitIdleDevice(struct Device_device * device);
VkFormat Device_findSupportedFormat(
	struct Device_State * device,
	const VkFormat* formats,
	uint32_t len,
	VkImageTiling tiling,
	VkFormatFeatureFlags2 features);

VkFormat Device_findDepthFormat(
	struct Device_State * device
);


uint32_t Device_findMemoryType(
	struct Device_State * device,
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties
);

void  Device_AllocateCommandBuffer(
	struct Device_State * device,
	VkCommandPool pool, 
	VkCommandBuffer* out
);

void  Device_createCommandPool(
	struct 	Device_State * device,
	struct Device_Queue queue,
	VkCommandPool *pool
);

void Device_CreateFence(struct Device_State * device,VkFence *fence);
void Device_DestroyFence(struct Device_State * device,VkFence fence);

VkCommandBuffer Device_beginSingleTimeCommands(struct Device_State* device);

void Device_endSingleTimeCommands(
	struct Device_State* device,
	VkCommandBuffer commandBuffer
);
