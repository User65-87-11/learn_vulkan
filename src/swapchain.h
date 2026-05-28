#pragma once
#include <vulkan/vulkan_core.h>
#include "src/device2.h"
#include "util/common.h"


struct Swapchain_info{
	struct Device_State * ref_device;
	struct Platform_State * ref_platform;
	VkExtent2D extent;
};
struct Swapchain {
	struct Device_State * ref_device;
	struct Platform_State * ref_platform;
	
	VkSwapchainKHR handle;

	VkFormat surfaceFormat;
	VkColorSpaceKHR colorSpace;
	VkFormat depthFormat;

	VkExtent2D extent;

	VkImage images[MAX_IMAGE_VIEWS];
	VkImageView image_views[MAX_IMAGE_VIEWS];
	uint32_t imageCount;

	VkPresentModeKHR presentMode;
};

void Swapchain_Create(
	struct Swapchain_info * info,
	struct Swapchain* sc
	// VkDevice device,
	// VkPhysicalDevice physicalDevice,
	// VkSurfaceKHR surface,
	// VkExtent2D extent
);

void Swapchain_Destroy(struct Swapchain* sc);

void Swapchain_Recreate(struct Swapchain* sc


);
