#pragma once
#include <vulkan/vulkan_core.h>
#include "util/common.h"
#include "swapchain.h"
#include "platform.h"
#include "device2.h"

static void createImageView(
		struct Swapchain * sc,VkImage image,
	VkFormat format,
	VkImageAspectFlagBits aspectFlags,
	VkImageView* view);

static void createImageViews(struct Swapchain* sc);

void Swapchain_Create(
	struct Swapchain_info * info,
	struct Swapchain* sc

)
{
	PRINT_FNAME;
	sc->ref_device = info->ref_device;
	sc->ref_platform = info->ref_platform;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	// VkPhysicalDevice physicalDevice = Device_GetPhysical();
	// VkDevice  device = Device_Get();

	// surface0 = surface;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		info->ref_device->physical_device, info->ref_platform->surface, &surfaceCapabilities);

	printf("Surface extent: w:%d h:%d\n",
		surfaceCapabilities.currentExtent.width,
		surfaceCapabilities.currentExtent.height);

	uint32_t physicalDeviceSurfaceFormatCount = 0;

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		info->ref_device->physical_device, info->ref_platform->surface, &physicalDeviceSurfaceFormatCount, NULL);

	VkSurfaceFormatKHR surfaceFormats[physicalDeviceSurfaceFormatCount];

	vkGetPhysicalDeviceSurfaceFormatsKHR(info->ref_device->physical_device, info->ref_platform->surface,
		&physicalDeviceSurfaceFormatCount, surfaceFormats);

	sc->depthFormat = Device_findDepthFormat(info->ref_device);
	sc->surfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;
	printf("Surface formats:\n");

	// swapchainSurfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;

	// swapchainSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	sc->colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	for (int i = 0; i < physicalDeviceSurfaceFormatCount; i++) {
		printf("\tSurfaceFormat.format: %d\n", surfaceFormats[i].format);

		if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) {

			sc->surfaceFormat = surfaceFormats[i].format;
		}
	}

	for (int i = 0; i < physicalDeviceSurfaceFormatCount; i++) {
		printf("\tSurfaceFormat.colorSpace:%d\n", surfaceFormats[i].colorSpace);

		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			sc->colorSpace = surfaceFormats[i].colorSpace;
		}
	}

	uint32_t presentModeCount = 0;

	vkGetPhysicalDeviceSurfacePresentModesKHR(
	info->ref_device->physical_device, info->ref_platform->surface, &presentModeCount, NULL);

	VkPresentModeKHR presentModes[presentModeCount];

	vkGetPhysicalDeviceSurfacePresentModesKHR(
	info->ref_device->physical_device, info->ref_platform->surface, &presentModeCount, presentModes);

	printf("Surface pPresentModes:\n");

	uint32_t presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

	for (int i = 0; i < presentModeCount; i++) {
		printf("\tpresent mode: %d\n", presentModes[i]);

		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}

	sc->extent = surfaceCapabilities.currentExtent;

	if (sc->extent.width > surfaceCapabilities.maxImageExtent.width &&
		sc->extent.height > surfaceCapabilities.maxImageExtent.height) {

		sc->extent.width = WIDTH;
		sc->extent.height = HEIGHT;
	}

	uint32_t width, height;
	Platform_GetFramebufferSize(info->ref_platform,&width, &height);

	if (width >= surfaceCapabilities.minImageExtent.width &&
		width <= surfaceCapabilities.maxImageExtent.width &&
		height >= surfaceCapabilities.minImageExtent.height &&
		height <= surfaceCapabilities.maxImageExtent.height) {
		sc->extent = (VkExtent2D){
			.width = width,
			.height = height,
		};
	}

	/*
			INFO

			Color attachments: at least 4–8 (depends on GPU)
			Depth/stencil: 1

			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			 - one per queue family
	*/

	uint32_t imageCount = 2;

	if (imageCount < surfaceCapabilities.minImageCount) {

		imageCount = surfaceCapabilities.minImageCount;
	}

	if (surfaceCapabilities.maxImageCount > 0 &&
		imageCount > surfaceCapabilities.maxImageCount) {

		imageCount = surfaceCapabilities.maxImageCount;
	}
	sc->imageCount = imageCount;

	VkSwapchainCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.minImageCount = sc->imageCount,
		.imageFormat = sc->surfaceFormat,
		.imageColorSpace = sc->colorSpace,
		.imageExtent = sc->extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = surfaceCapabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = presentMode,
		.surface = info->ref_platform->surface,
		.clipped = true,
	};

	vkCreateSwapchainKHR(info->ref_device->logical_device, &createInfo, NULL, &sc->handle);

	createImageViews(sc);
}

static void createImageViews(struct Swapchain* sc) {

	PRINT_FNAME;

	/*
	INFO

	how many images you want (minImageCount)
	how they should be formatted (imageFormat)
	how they will be used (imageUsage)
	their size (imageExtent)

	*/

	// VkDevice device = Device_Get();

	vkGetSwapchainImagesKHR(sc->ref_device->logical_device, sc->handle, &sc->imageCount, NULL);

	// VkImage sc_images[image_view_cnt];

	vkGetSwapchainImagesKHR(sc->ref_device->logical_device, sc->handle, &sc->imageCount, sc->images);

	printf("pSwapchainImageCount %d\n", &sc->imageCount);

	// swapchainImageViewCount = swapchainImageCount;

	for (int i = 0; i < sc->imageCount; i++) {

		// swapchain_images[i].handle = sc_images[i];
		// swapchain_images[i].mipLevels = 1;
		createImageView(sc,sc->images[i], sc->surfaceFormat,
			VK_IMAGE_ASPECT_COLOR_BIT, &sc->image_views[i]);
	}
}

static void createImageView(
	struct Swapchain * sc,
	VkImage image,
	VkFormat format,
	VkImageAspectFlagBits aspectFlags,
	VkImageView* view) {

	VkImageViewCreateInfo viewInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
		.subresourceRange =
			{
				.aspectMask = aspectFlags,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},

	};

	vkCreateImageView(sc->ref_device->logical_device, &viewInfo, NULL, view);
}

void Swapchain_Destroy(struct Swapchain* sc) {
	PRINT_FNAME;
	// struct Device* device = Device_Get();

	vkDeviceWaitIdle(sc->ref_device->logical_device);

	for (int i = 0; i < sc->imageCount; i++) {
		// cleanImageRes(&swapchain_images[i]);
		vkDestroyImageView(sc->ref_device->logical_device, sc->image_views[i], NULL);
	}
	// swapchainImageViewCount = 0;
	sc->imageCount = 0;

	vkDestroySwapchainKHR(sc->ref_device->logical_device, sc->handle, NULL);

	sc->handle = NULL;
}

void Swapchain_Recreate(struct  Swapchain_info * info,struct Swapchain* sc

) {
	PRINT_FNAME;
	PRINT_FNAME;


	// Destroy old swapchain
	Swapchain_Destroy(sc);
	
	Swapchain_Create(info,sc);
}
