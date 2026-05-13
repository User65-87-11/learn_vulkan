#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <string.h>
#include "device.h"
#include "util/common.h"
#include "platform.h"
#include "instance.h"


static struct Device device;


// static VkCommandBuffer transferCommandBuffer;
// static VkFence transferFence;

static  uint32_t requiredDeviceExtensionCnt = 1;

static  const char *requiredDeviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};



static void createPhysicalDevice();
static void createLogicalDevice() ;
static void physicalDeviceExtensionCheck();
static void physicalDeviceFeatureCheck();

static void createQueue();
static void createTransferFence();



static void createCommandPool();

void Device_Create(){

	createPhysicalDevice();
	
	physicalDeviceExtensionCheck();
	
	physicalDeviceFeatureCheck();
	
	createLogicalDevice();

	createQueue();

	createCommandPool();

	

	createTransferFence();



	
}
void Device_Destroy(void){
	PRINT_FNAME;
	
	

	
	
	vkDestroyCommandPool(device.logical_device, device.transfer_pool, NULL);
	vkDestroyCommandPool(device.logical_device, device.graphics_pool, NULL);
	vkDestroyFence(device.logical_device, device.transfer_fence, NULL);
	
	if(device.logical_device != NULL)
	{
		vkDestroyDevice(device.logical_device, NULL);
		device.logical_device = NULL;
	}
}

static void createTransferFence(){
	VkFenceCreateInfo fenceInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
	};
	vkCreateFence(device.logical_device, &fenceInfo, NULL, &device.transfer_fence);

}

VkCommandBuffer Device_createTransferCommandBuffer(){

	VkCommandBuffer command; 
	Device_AllocateCommandBuffer(device.transfer_pool,&command);
	return command;
}

void Device_WaitIdle(){
	vkDeviceWaitIdle(device.logical_device);
}

struct Device * Device_Get(void){
	return &device;
}
VkFormat Device_findSupportedFormat(
    const VkFormat *formats,
    uint32_t len,
    VkImageTiling tiling,
    VkFormatFeatureFlags2 features 
) 
{
	// VkPhysicalDevice physicalDevice = getPhysicalDevice();

	for (uint32_t i = 0; i < len; i++) {

        VkFormatProperties2 props = {
            .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2
        };

        vkGetPhysicalDeviceFormatProperties2(
            device.physical_device,
            formats[i],
            &props
        );

        VkFormatFeatureFlags2 supported =
            (tiling == VK_IMAGE_TILING_LINEAR)
            ? props.formatProperties.linearTilingFeatures
            : props.formatProperties.optimalTilingFeatures;

        if ((supported & features) == features) {
            return formats[i];
        }
    }

    EXIT_CLEAN("failed to find supported format!");
    return VK_FORMAT_UNDEFINED;
}

VkFormat Device_findDepthFormat() {

	VkFormat formats[] = {
		VK_FORMAT_D32_SFLOAT, 
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT
	};
	
	return Device_findSupportedFormat(
		formats, 
		ARR_LEN(formats), 
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}


static void physicalDeviceExtensionCheck() {

  PRINT_FNAME;

  uint32_t deviceExtensionPropertieCount;
  vkEnumerateDeviceExtensionProperties(
	  device.physical_device, 
	  NULL,
	  &deviceExtensionPropertieCount, 
	  NULL
  );

  VkExtensionProperties exp_props[deviceExtensionPropertieCount];
  
  vkEnumerateDeviceExtensionProperties(
      device.physical_device, 
      NULL, 
      &deviceExtensionPropertieCount, 
      exp_props
  );

  int supportedCnt = 0;

  for (int i = 0; i < deviceExtensionPropertieCount; i++) {

    for (int k = 0; k < requiredDeviceExtensionCnt; k++) {
    	if (strcmp(exp_props[i].extensionName, requiredDeviceExtensions[k]) == 0) {
			printf("\tExtension found %s\n", requiredDeviceExtensions[k]);
			supportedCnt++;
			break;
     	}
	}
  }
	
	if (!(supportedCnt == requiredDeviceExtensionCnt)) {
		printf("No suported extensions\n");
		printf("supportedCnt: %d of %d\n", supportedCnt,requiredDeviceExtensionCnt);
	
	for (int i = 0; i < deviceExtensionPropertieCount; i++) {
	
		printf("\tfound: %s\n", exp_props[i].extensionName);
	}
	for (int i = 0; i < requiredDeviceExtensionCnt; i++) {
	
		printf("\trequired: %s\n", requiredDeviceExtensions[i]);
	}
	
		EXIT_CLEAN("\n");
	}
}

static void physicalDeviceFeatureCheck() {

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

  vkGetPhysicalDeviceFeatures2(device.physical_device, &physicalDeviceFeatures2);

  VkPhysicalDeviceFeatures2 *next = &physicalDeviceFeatures2;

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
      }

      break;
    case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT:
      printf("supported: "
             "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_"
             "FEATURES_EXT\n");

      {
        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT *temp =
            (VkPhysicalDeviceExtendedDynamicStateFeaturesEXT *)next;
        if (temp->extendedDynamicState) {
          all_ok++;
        }
      }

      break;
    case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES:
      printf(
          "supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES\n");

      {
        VkPhysicalDeviceVulkan13Features *temp =
            (VkPhysicalDeviceVulkan13Features *)next;
        if (temp->dynamicRendering && temp->synchronization2) {
          all_ok += 2;
        }
      }
      break;
    case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES:
      printf(
          "supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES\n");

      {
        VkPhysicalDeviceVulkan12Features *temp =
            (VkPhysicalDeviceVulkan12Features *)next;
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
  if (all_ok != 6) {

    printf("supported :%d \n", all_ok);

    EXIT_CLEAN("Some Device features not supported");
  }
  
}

static uint32_t find_graphics_family(VkPhysicalDevice physical_device)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);

    VkQueueFamilyProperties* families =
        malloc(sizeof(VkQueueFamilyProperties) * count);

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, families);

    for (uint32_t i = 0; i < count; i++)
    {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            free(families);
            return i;
        }
    }

    free(families);

    EXIT_CLEAN("find_graphics_family cant find index ");
    
    return UINT32_MAX; // or invalid sentinel
}
static uint32_t find_presentation_family(
    VkPhysicalDevice physical_device,
    VkSurfaceKHR surface
)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);

    for (uint32_t i = 0; i < count; i++)
    {
        VkBool32 supported = VK_FALSE;

        vkGetPhysicalDeviceSurfaceSupportKHR(
            physical_device,
            i,
            surface,
            &supported
        );

        if (supported)
            return i;
    }

    EXIT_CLEAN("find_presentation_family cant find index ");
    return UINT32_MAX;
}
static uint32_t find_transfer_family(VkPhysicalDevice physical_device)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);

    VkQueueFamilyProperties families[32];
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, families);

    for (uint32_t i = 0; i < count; i++)
    {
        if (families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            return i;
        }
    }

    EXIT_CLEAN("find_transfer_family cant find index ");
    return UINT32_MAX;
}

static void createLogicalDevice() {

  PRINT_FNAME;

  device.graphics_family = find_graphics_family(device.physical_device);
  device.transfer_family = find_transfer_family(device.physical_device);
  device.present_family = find_presentation_family(device.physical_device, Platform_GetSurface());
  

  float queuePriority = 1.0f;
  
  VkDeviceQueueCreateInfo queueInfos[3];
  uint32_t queueCount = 0;
  

  queueInfos[queueCount++] = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = device.graphics_family,
      .queueCount = 1,
      .pQueuePriorities = &queuePriority
  };
  

  if (device.transfer_family != device.graphics_family)
  {
      queueInfos[queueCount++] = (VkDeviceQueueCreateInfo){
          .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          .queueFamilyIndex = device.transfer_family,
          .queueCount = 1,
          .pQueuePriorities = &queuePriority
      };
  }
  

  if (device.present_family != device.graphics_family &&
      device.present_family != device.transfer_family)
  {
      queueInfos[queueCount++] = (VkDeviceQueueCreateInfo){
          .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          .queueFamilyIndex = device.present_family,
          .queueCount = 1,
          .pQueuePriorities = &queuePriority
      };
  }
  

  VkPhysicalDeviceFeatures2 physicalDeviceFeatures2 = {

      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
      .features.samplerAnisotropy = VK_TRUE,

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
  physicalDeviceFeatures13.pNext = &physicalDeviceExtendedDynamicStateFeaturesEXT;
  physicalDeviceFeatures12.pNext = &physicalDeviceFeatures13;
  physicalDeviceFeatures2.pNext = &physicalDeviceFeatures12;

  VkDeviceCreateInfo deviceCreateInfo = {

      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pQueueCreateInfos = queueInfos,
      .queueCreateInfoCount = queueCount,
      .pNext = &physicalDeviceFeatures2,
      .ppEnabledExtensionNames = requiredDeviceExtensions,
      .enabledExtensionCount = requiredDeviceExtensionCnt,
  };

  VkResult res = vkCreateDevice(device.physical_device, &deviceCreateInfo, NULL, &device.logical_device);

  if (res != VK_SUCCESS) {
    EXIT_CLEAN("failed to create logical device");
  }
}

static void createPhysicalDevice() {
	PRINT_FNAME;

	VkInstance instance = Instance_Get();
	
	uint32_t physicalDeviceCount = 0;
	
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL);
	
	VkPhysicalDevice physicalDevices[physicalDeviceCount];
	
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);
	
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
	
		EXIT_CLEAN("failed to find a suitable GPU!");
	}
	device.physical_device = physicalDevices[max_id];
	
	vkGetPhysicalDeviceFeatures(device.physical_device, &features);
	
	vkGetPhysicalDeviceProperties(device.physical_device, &properties);
	
	printf("\nSELECTED device name: %s\n", properties.deviceName);
	printf("device rating: %d\n", ratings[max_id]);
	printf("device type: %d\n", properties.deviceType);
	printf("geometry shader: %d\n", features.geometryShader);
	bool supportsVulkan1_3 = properties.apiVersion >= VK_VERSION_1_3;
	printf("API >= 1.3 support: %d\n", supportsVulkan1_3);
	printf("\n");
}

VkBool32 debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
	void *pUserData
) 
{
	
	printf("[validation layer]: %d %d %s\n", messageSeverity, messageTypes,
	        pCallbackData->pMessage);
	return VK_FALSE;
}


static void createQueue() {

	vkGetDeviceQueue(
        device.logical_device,
        device.graphics_family,
        0,
        &device.graphics_queue
    );

    vkGetDeviceQueue(
        device.logical_device,
        device.transfer_family,
        0,
        &device.transfer_queue
    );
}

// static void createCommandPool(VkDevice device,
//                               uint32_t graphicsFamily,
//                               uint32_t transferFamily)


static void createCommandPool() 
{
	PRINT_FNAME;



  
	VkCommandPoolCreateInfo commandPoolCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = device.graphics_family,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	 VkResult result = vkCreateCommandPool(
		device.logical_device, 
		&commandPoolCreateInfo, 
		NULL,
		&device.graphics_pool
	);
		
	if (result != VK_SUCCESS) {
        EXIT_CLEAN("Failed to create graphics command pool");
    }

	
	VkCommandPoolCreateInfo commandPoolCreateInfo2 = (VkCommandPoolCreateInfo){
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = device.transfer_family,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	
	 result = vkCreateCommandPool(
		device.logical_device, 
		&commandPoolCreateInfo2, 
		NULL,
		&device.transfer_pool
	);
	if (result != VK_SUCCESS) {
        EXIT_CLEAN("Failed to create graphics command pool");
    }
  //fix
		// for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		// {
		// 	struct FrameData *frame = &g_frames[i];
			
	
		// 	VkCommandBufferAllocateInfo allocInfo = {
		// 		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		// 		.commandPool = graphicsPool,
		// 		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		// 		.commandBufferCount = 1,
		// 	};
			
		// 	vkAllocateCommandBuffers(
		// 		device, 
		// 		&allocInfo,
		// 		&
		// 	);
		// }


  // VkCommandBufferAllocateInfo allocInfo = (VkCommandBufferAllocateInfo){
  //     .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
  //     .commandPool = transferCommnadPool,
  //     .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
  //     .commandBufferCount = 1,

  // };

  // vkAllocateCommandBuffers(device, &allocInfo, &transferCommandBuffers);
}
void Device_AllocateCommandBuffer(VkCommandPool pool, VkCommandBuffer * out){
	VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer cmd;
    VkResult res = vkAllocateCommandBuffers(device.logical_device, &allocInfo, out);

    if (res != VK_SUCCESS) {
        EXIT_CLEAN("Failed to allocate command buffer");
    }

   
}

uint32_t Device_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
  vkGetPhysicalDeviceMemoryProperties(device.physical_device,
                                      &physicalDeviceMemoryProperties);

  for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) &&
        (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags &
         properties) == properties) {
      return i;
    }
  }

  EXIT_CLEAN("failed to find suitable memory type!");
}

