#pragma once
#include <vulkan/vulkan_core.h>


struct Device {
    VkPhysicalDevice physical_device;
    VkDevice logical_device;

    VkQueue graphics_queue;
    VkQueue transfer_queue;
    VkQueue present_queue;

    uint32_t graphics_family;
    uint32_t transfer_family;
    uint32_t present_family;
    

    VkCommandPool graphics_pool;
    VkCommandPool transfer_pool;
    VkCommandBuffer transfer_cmd_buffer;
    VkFence transfer_fence;
    
};

void Device_Create();
void Device_Destroy(void);

struct Device *  Device_Get(void);
// VkPhysicalDevice Device_GetPhysical(void);

void Device_WaitIdle();



VkFormat Device_findDepthFormat();

// VkCommandPool Device_GetGraphicsCommandPool();
// VkCommandPool Device_GetTransferCommandPool();

void Device_AllocateCommandBuffer(VkCommandPool pool, VkCommandBuffer * out);


uint32_t Device_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

VkFormat Device_findSupportedFormat(
	VkFormat *formats, uint32_t len,
	VkImageTiling tiling,
	VkFormatFeatureFlags features
);