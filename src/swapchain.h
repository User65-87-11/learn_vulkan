#pragma once
#include <vulkan/vulkan_core.h>
#include "util/common.h"

struct Swapchain{
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
    struct Swapchain* sc,
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    VkSurfaceKHR surface,
    VkExtent2D extent
);

void Swapchain_Destroy(struct Swapchain* sc);

void Swapchain_Recreate(
    struct Swapchain* sc,
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    VkSurfaceKHR surface
 
);
