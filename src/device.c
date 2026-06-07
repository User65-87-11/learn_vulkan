#include <GLFW/glfw3.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

#include "device.h"
#include "instance.h"
#include "platform.h"
#include "common.h"


static void createPhysicalDevice(
	struct Device_CreateInfo* info,
	struct Device_State* device
);
static void createLogicalDevice(
	struct Device_CreateInfo* info,
	struct Device_State* device
);
static void physicalDeviceExtensionCheck(
	struct Device_CreateInfo* info,
	struct Device_State* device
);
static void physicalDeviceFeatureCheck(
	struct Device_CreateInfo* info,
	struct Device_State* device
);
static VkCommandBuffer createTransferCommandBuffer(struct Device_State* device);

static void createQueues(struct Device_State* device);

static void createTransferFence(struct Device_State* device, VkFence* fence);

// static void createCommandPool();



void Device_Create(struct Device_CreateInfo * info, struct Device_State * device) {
	PRINT_FNAME;
	createPhysicalDevice(info,device);

	physicalDeviceExtensionCheck(info,device);

	physicalDeviceFeatureCheck(info,device);

	createLogicalDevice(info,device);

	// device->depth_format = Device_findDepthFormat(device);
	
	createQueues(device);

	Device_createCommandPool(device, device->queue_graphics,&device->command_pool_graphics);
	
	Device_createCommandPool(device, device->queue_tranfser,&device->command_pool_transfer);

	createTransferFence(device,&device->transfer_fence);
}

void Device_Destroy(struct Device_State * device) {
	PRINT_FNAME;

	vkDestroyCommandPool(device->logical_device, device->command_pool_transfer, NULL);
	
	vkDestroyCommandPool(device->logical_device, device->command_pool_graphics, NULL);

	vkDestroyFence(device->logical_device, device->transfer_fence, NULL);

	vkDestroyDevice(device->logical_device, NULL);

}

static void createTransferFence(struct Device_State *device, VkFence *fence) {
	VkFenceCreateInfo fenceInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
	vkCreateFence(device->logical_device, &fenceInfo, NULL, fence);
}

VkCommandBuffer Device_beginSingleTimeCommands(struct Device_State* device) {

	VkCommandBuffer commandBuffer = createTransferCommandBuffer(device);

	// vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Device_endSingleTimeCommands(
	struct Device_State* device,
	VkCommandBuffer commandBuffer
)
{

	// VkDevice device = Device_Get()->device;

	vkEndCommandBuffer(commandBuffer);

	// VkPipelineStageFlags2 stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;

	VkCommandBufferSubmitInfo buffersubmitInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = commandBuffer
	};

	VkSubmitInfo2 submitInfo2 = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &buffersubmitInfo

	};

	vkResetFences(device->logical_device, 1, &device->transfer_fence);

	vkQueueSubmit2(
		device->queue_tranfser.queue, 1, &submitInfo2, device->transfer_fence
	);

	vkWaitForFences(
		device->logical_device, 1, &device->transfer_fence, VK_TRUE, UINT64_MAX
	);

	vkFreeCommandBuffers(
		device->logical_device, device->command_pool_transfer, 1, &commandBuffer
	);
}

static VkCommandBuffer createTransferCommandBuffer(struct Device_State * device) {

	VkCommandBuffer command;
	Device_AllocateCommandBuffer(device, device->command_pool_transfer, &command);
	return command;
}

void Device_WaitIdleDevice(struct Device_State *device) {
	vkDeviceWaitIdle(device->logical_device);
}

// struct Device* Device_Get(void) { return &device; }

VkFormat Device_findSupportedFormat(
	struct Device_State * device,
	const VkFormat* formats,
	uint32_t len,
	VkImageTiling tiling,
	VkFormatFeatureFlags2 features) {
	// VkPhysicalDevice physicalDevice = getPhysicalDevice();

	for (uint32_t i = 0; i < len; i++) {

		VkFormatProperties2 props = {
			.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2};

		vkGetPhysicalDeviceFormatProperties2(
			device->physical_device, formats[i], &props);

		VkFormatFeatureFlags2 supported =
			(tiling == VK_IMAGE_TILING_LINEAR)
				? props.formatProperties.linearTilingFeatures
				: props.formatProperties.optimalTilingFeatures;

		if ((supported & features) == features) {
			return formats[i];
		}
	}

	EXIT_PRINT("failed to find supported format!");
	return VK_FORMAT_UNDEFINED;
}

VkFormat Device_findDepthFormat(
	struct Device_State * device
) {

	VkFormat formats[] = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT};

	return Device_findSupportedFormat(
		device,formats, ARR_LEN(formats),
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

static void physicalDeviceExtensionCheck(struct Device_CreateInfo * info,struct Device_State * device) {
	PRINT_FNAME;

	uint32_t deviceExtensionPropertieCount;
	vkEnumerateDeviceExtensionProperties(
		device->physical_device, NULL, &deviceExtensionPropertieCount, NULL);

	VkExtensionProperties exp_props[deviceExtensionPropertieCount];

	vkEnumerateDeviceExtensionProperties(device->physical_device, NULL,
		&deviceExtensionPropertieCount, exp_props);

	int supportedCnt = 0;

	for (int i = 0; i < deviceExtensionPropertieCount; i++) {

		for (int k = 0; k < info->requiredDeviceExtensions_cnt; k++) {
			if (strcmp(exp_props[i].extensionName,
					info->requiredDeviceExtensions[k]) == 0) {
				printf("\tExtension found %s\n", info->requiredDeviceExtensions[k]);
				supportedCnt++;
				break;
			}
		}
	}

	if (!(supportedCnt == info->requiredDeviceExtensions_cnt)) {
		printf("No suported extensions\n");
		printf("supportedCnt: %d of %d\n", supportedCnt,
			info->requiredDeviceExtensions_cnt);

		for (int i = 0; i < deviceExtensionPropertieCount; i++) {

			printf("\tfound: %s\n", exp_props[i].extensionName);
		}
		for (int i = 0; i < info->requiredDeviceExtensions_cnt; i++) {

			printf("\trequired: %s\n", info->requiredDeviceExtensions[i]);
		}

		EXIT_PRINT("\n");
	}
}

static void physicalDeviceFeatureCheck(struct Device_CreateInfo * info ,struct Device_State * device) {

	PRINT_FNAME;



	
	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		
	};

	VkPhysicalDeviceVulkan12Features features12 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		
	};

	VkPhysicalDeviceVulkan13Features features13 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,

	};

	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT
		physicalDeviceExtendedDynamicStateFeaturesEXT = {

			.sType =
				VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
		};

	features13.pNext = &physicalDeviceExtendedDynamicStateFeaturesEXT;
	features12.pNext = &features13;
	physicalDeviceFeatures2.pNext = &features12;

	vkGetPhysicalDeviceFeatures2(
		device->physical_device, &physicalDeviceFeatures2);

	VkPhysicalDeviceFeatures2* next = &physicalDeviceFeatures2;

	int all_ok = 0;

	while (next != NULL) {

		switch (next->sType) {
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:
			printf("supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2\n");

			{

				if (next->features.samplerAnisotropy) {
					all_ok++;
				}
				if (next->features.vertexPipelineStoresAndAtomics) {
					all_ok++;
				}
				if (next->features.fragmentStoresAndAtomics) {
					all_ok++;
				}
			
			
			}

			break;
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT:
			printf("supported: "
				   "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_"
				   "FEATURES_EXT\n");

			{
				VkPhysicalDeviceExtendedDynamicStateFeaturesEXT* temp =
					(VkPhysicalDeviceExtendedDynamicStateFeaturesEXT*)next;
				if (temp->extendedDynamicState) {
					all_ok++;
				}
			}

			break;
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES:
			printf("supported: "
				   "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES\n");

			{
				VkPhysicalDeviceVulkan13Features* temp =
					(VkPhysicalDeviceVulkan13Features*)next;
				if (temp->dynamicRendering && temp->synchronization2) {
					all_ok += 2;
				}
			}
			break;
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES:
			printf("supported: "
				   "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES\n");

			{
				VkPhysicalDeviceVulkan12Features* temp =
					(VkPhysicalDeviceVulkan12Features*)next;
				if (temp->runtimeDescriptorArray) {
					all_ok += 1;
				}
			}
			break;
		default:
			printf("next %d\n", next->sType);
			break;
		}
		next = next->pNext;
	}
	if (all_ok != 7) {

		printf("supported :%d \n", all_ok);

		EXIT_PRINT("Some Device features not supported");
	}
}

static uint32_t find_graphics_family(VkPhysicalDevice physical_device) {
	uint32_t count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);

	VkQueueFamilyProperties* families =
		malloc(sizeof(VkQueueFamilyProperties) * count);

	
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, families);

	for (uint32_t i = 0; i < count; i++) {
		if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			free(families);
			return i;
		}
	}

	free(families);

	EXIT_PRINT("find_graphics_family cant find index ");

	return UINT32_MAX; // or invalid sentinel
}

static uint32_t find_presentation_family(
	VkPhysicalDevice physical_device, 
	VkSurfaceKHR surface
) {
	uint32_t count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);

	for (uint32_t i = 0; i < count; i++) {
		VkBool32 supported = VK_FALSE;

		vkGetPhysicalDeviceSurfaceSupportKHR(
			physical_device, i, surface, &supported);

		if (supported)
			return i;
	}

	EXIT_PRINT("find_presentation_family cant find index ");
	return UINT32_MAX;
}

static uint32_t find_transfer_family(VkPhysicalDevice physical_device) {
	uint32_t count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);

	VkQueueFamilyProperties families[32];
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, families);

	for (uint32_t i = 0; i < count; i++) {
		if (families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			return i;
		}
	}

	EXIT_PRINT("find_transfer_family cant find index ");
	return UINT32_MAX;
}

static void createLogicalDevice(struct Device_CreateInfo *into ,struct Device_State * device) {

	PRINT_FNAME;

	device->queue_graphics.index = find_graphics_family(device->physical_device);
	device->queue_tranfser.index = find_transfer_family(device->physical_device);
	device->queue_present.index =
		find_presentation_family(device->physical_device,into->ref_platform->surface);

	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo queueInfos[3];
	uint32_t queueCount = 0;

	queueInfos[queueCount++] = (VkDeviceQueueCreateInfo){
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = device->queue_graphics.index,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority};

	if (device->queue_tranfser.index != device->queue_graphics.index) {
		queueInfos[queueCount++] = (VkDeviceQueueCreateInfo){
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = device->queue_tranfser.index,
			.queueCount = 1,
			.pQueuePriorities = &queuePriority};
	}

	if (device->queue_present.index != device->queue_graphics.index &&
		device->queue_present.index != device->queue_tranfser.index) {
		queueInfos[queueCount++] = (VkDeviceQueueCreateInfo){
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = device->queue_present.index,
			.queueCount = 1,
			.pQueuePriorities = &queuePriority};
	}

	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2 = {

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.features.samplerAnisotropy = VK_TRUE,
		.features.fragmentStoresAndAtomics = VK_TRUE,

	};

	VkPhysicalDeviceVulkan12Features physicalDeviceFeatures12 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.runtimeDescriptorArray = VK_TRUE,
	};

	VkPhysicalDeviceVulkan13Features physicalDeviceFeatures13 = {

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.dynamicRendering = VK_TRUE,
		.synchronization2 = VK_TRUE,

	};

	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT
		physicalDeviceExtendedDynamicStateFeaturesEXT = {

			.sType =
				VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
			.extendedDynamicState = VK_TRUE,

		};

	physicalDeviceExtendedDynamicStateFeaturesEXT.pNext = NULL;
	physicalDeviceFeatures13.pNext =
		&physicalDeviceExtendedDynamicStateFeaturesEXT;
	physicalDeviceFeatures12.pNext = &physicalDeviceFeatures13;
	physicalDeviceFeatures2.pNext = &physicalDeviceFeatures12;

	VkDeviceCreateInfo deviceCreateInfo = {

		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = queueInfos,
		.queueCreateInfoCount = queueCount,
		.pNext = &physicalDeviceFeatures2,
		.ppEnabledExtensionNames = into->requiredDeviceExtensions,
		.enabledExtensionCount = into->requiredDeviceExtensions_cnt,
	};

	VkResult res = vkCreateDevice(device->physical_device, &deviceCreateInfo,
		NULL, &device->logical_device);

	if (res != VK_SUCCESS) {
		EXIT_PRINT("failed to create logical device");
	}
}

static void createPhysicalDevice(struct Device_CreateInfo* info, struct Device_State * device) {
	PRINT_FNAME;

	// VkInstance instance = Instance_Get();

	uint32_t physicalDeviceCount = 0;

	vkEnumeratePhysicalDevices(info->ref_instance->instance, &physicalDeviceCount, NULL);

	VkPhysicalDevice physicalDevices[physicalDeviceCount];

	vkEnumeratePhysicalDevices(info->ref_instance->instance, &physicalDeviceCount, physicalDevices);

	VkPhysicalDeviceFeatures features;

	VkPhysicalDeviceProperties properties;

	uint32_t ratings[physicalDeviceCount] = {};

	for (int i = 0; i < physicalDeviceCount; i++) {

		vkGetPhysicalDeviceFeatures(physicalDevices[i], &features);

		vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

		printf("\ndevice name: %s\n", properties.deviceName);

		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {

			ratings[i] += 1000;
		} else if (properties.deviceType ==
				   VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {

			ratings[i] += 500;
		}

		ratings[i] += properties.limits.maxImageDimension2D;

		if (!features.geometryShader) {
			ratings[i] = 0;
			continue;
		}
	}

	uint32_t max = 1;

	uint32_t max_id = 1000;

	for (int i = 0; i < physicalDeviceCount; i++) {

		if (max < ratings[i]) {

			max_id = i;

			max = ratings[i];
		}
	}

	if (max_id == 1000) {

		EXIT_PRINT("failed to find a suitable GPU!");
	}
	device->physical_device = physicalDevices[max_id];

	vkGetPhysicalDeviceFeatures(device->physical_device, &features);

	vkGetPhysicalDeviceProperties(device->physical_device, &properties);

	
	
	printf("\nSELECTED device name: %s\n", properties.deviceName);
	printf("device rating: %d\n", ratings[max_id]);
	printf("device type: %d\n", properties.deviceType);
	printf("geometry shader: %d\n", features.geometryShader);
	printf("alignment UBO: %d\n", properties.limits.minUniformBufferOffsetAlignment);
	printf("alignment SSBO: %d\n", properties.limits.minStorageBufferOffsetAlignment);
	bool supportsVulkan1_3 = properties.apiVersion >= VK_VERSION_1_3;
	printf("API >= 1.3 support: %d\n", supportsVulkan1_3);
	printf("\n");
}

VkBool32 debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {

	printf(
		"[validation layer]: %d %d %s\n", 
		messageSeverity, 
		messageTypes,
		pCallbackData->pMessage
	);
	return VK_FALSE;
}

static void createQueues(struct Device_State * device) {

	vkGetDeviceQueue(
		device->logical_device, 
		device->queue_graphics.index, 0,
		&device->queue_graphics.queue
	);

	vkGetDeviceQueue(
		device->logical_device, 
		device->queue_tranfser.index, 0,
		&device->queue_tranfser.queue
	);
}

void  Device_createCommandPool(
	struct 	Device_State * device,
	struct Device_Queue queue,
	VkCommandPool *pool
)
{

	PRINT_FNAME;

	VkCommandPoolCreateInfo commandPoolCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = queue.index,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};

	VK_CHECK(vkCreateCommandPool(
		device->logical_device,
		&commandPoolCreateInfo, NULL, pool));


}

void  Device_AllocateCommandBuffer(
	struct Device_State * device,
	VkCommandPool pool, 
	VkCommandBuffer* out
) {
	VkCommandBufferAllocateInfo allocInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1};

	VkCommandBuffer cmd;
	VkResult res =
		vkAllocateCommandBuffers(device->logical_device, &allocInfo, out);

	if (res != VK_SUCCESS) {
		EXIT_PRINT("Failed to allocate command buffer");
	}
}

uint32_t Device_findMemoryType(
	struct Device_State * device,
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties
) {
	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(
		device->physical_device, &physicalDeviceMemoryProperties);

	for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) &&
			(physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags &
				properties) == properties) {
			return i;
		}
	}

	EXIT_PRINT("failed to find suitable memory type!");
}

void Device_CreateFence(struct Device_State * device,VkFence *fence)
{
	VkFenceCreateInfo fenceInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
	};
	vkCreateFence(
		device->logical_device, 
		&fenceInfo, 
		NULL, 
		fence
	);
}
void Device_DestroyFence(struct Device_State * device,VkFence fence){
	vkDestroyFence(device->logical_device, fence, NULL);
}
