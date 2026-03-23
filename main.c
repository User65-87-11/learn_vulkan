#include "cglm/types.h"
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "cglm/cglm.h"



const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#define PRINT_FNAME printf("Call to: %s\n",__FUNCTION__)

#define EXIT_CLEAN(msg){\
		printf("ERROR: %s\n",msg);\
		cleanup();\
		exit(1);\
	}while(0)\

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#define MAX_IMAGE_VIEWS 16

#define MAX_FRAMES_IN_FLIGHT 2


const uint32_t validationLayerCount = 1;

const char* validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
};

const uint32_t requiredDeviceExtensionCount = 1;

const char* requiredDeviceExtensions[] = {

        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// bool requiredDeviceExtensionSupport = false;

// bool requiredDeviceFeaturesSupport = false;

// bool transferQueueSupport = false;

// bool graphicsQueueSupport = false;
 
bool framebufferResized = false;

GLFWwindow* window = NULL;

VkInstance instance = NULL;

VkDebugUtilsMessengerEXT debugUtilsMessengerEXT = NULL;

VkPhysicalDevice physicalDevice = NULL;

VkDevice device = NULL;

// VkQueue bothQueue = NULL;

// uint32_t bothQueueFamilyIndex = -1;




VkQueue graphicsQueue = NULL;

VkQueue transferQueue = NULL;


uint32_t queueFamilyIndexCount = 2;

uint32_t graphicsQueueFamilyArrayIndex = 0;

uint32_t transferQueueFamilyArrayIndex = 1;

uint32_t queueFamilyIndeces[2] = {
	-1,-1,
};



uint32_t presentationSupportQueueFamilyIndex = - 1;




VkSurfaceKHR surface = NULL; 

VkSurfaceCapabilitiesKHR surfaceCapabilities ;

VkExtent2D imageExtent ;

uint32_t swapchainSurfaceFormat = -1;

uint32_t swapchainSurfaceColorSpace = -1;

VkSwapchainKHR swapchain = NULL;

uint32_t swapchainImageCount = 0 ;

VkImage swapchainImages[MAX_IMAGE_VIEWS];

uint32_t swapchainImageViewCount = 0;

VkImageView swapchainImageViews[MAX_IMAGE_VIEWS];

VkShaderModule shaderModuleFrag = NULL;

VkShaderModule shaderModuleVert = NULL;

VkPipelineLayout pipelineLayout = NULL;

VkPipeline graphicsPipeline = NULL;

uint32_t commandBufferCount = MAX_FRAMES_IN_FLIGHT;

uint32_t frameIndex = 0;

VkCommandBuffer graphicsCommandBuffers[MAX_FRAMES_IN_FLIGHT];


uint32_t transferCommandBuffersCount = 1;

VkCommandBuffer transferCommandBuffers ;

VkCommandPool graphicsCommnadPool = NULL;

VkCommandPool transferCommnadPool = NULL;

VkSemaphore presentCompleteSemaphore[MAX_FRAMES_IN_FLIGHT];

VkSemaphore renderFinishedSemaphore[MAX_FRAMES_IN_FLIGHT];

VkFence inFlightFences [MAX_FRAMES_IN_FLIGHT];

VkRenderPass renderPass = NULL;




//--------------------------


struct Vertex{

	vec2 pos;
	vec3 col;
};

struct Vertex vertices[3] = {
	{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
};

VkBuffer vertextBuffer = NULL;

VkDeviceMemory deviceMemory = NULL;

/**

                                                                                                                                                                         
                                           bbbbbbbb                                                                                                              dddddddd
                   lllllll                 b::::::b                              lllllll                                                                         d::::::d
                   l:::::l                 b::::::b                              l:::::l                                                                         d::::::d
                   l:::::l                 b::::::b                              l:::::l                                                                         d::::::d
                   l:::::l                  b:::::b                              l:::::l                                                                         d:::::d 
   ggggggggg   gggggl::::l    ooooooooooo   b:::::bbbbbbbbb      aaaaaaaaaaaaa    l::::l     ssssssssss            eeeeeeeeeeee    nnnn  nnnnnnnn        ddddddddd:::::d 
  g:::::::::ggg::::gl::::l  oo:::::::::::oo b::::::::::::::bb    a::::::::::::a   l::::l   ss::::::::::s         ee::::::::::::ee  n:::nn::::::::nn    dd::::::::::::::d 
 g:::::::::::::::::gl::::l o:::::::::::::::ob::::::::::::::::b   aaaaaaaaa:::::a  l::::l ss:::::::::::::s       e::::::eeeee:::::een::::::::::::::nn  d::::::::::::::::d 
g::::::ggggg::::::ggl::::l o:::::ooooo:::::ob:::::bbbbb:::::::b           a::::a  l::::l s::::::ssss:::::s     e::::::e     e:::::enn:::::::::::::::nd:::::::ddddd:::::d 
g:::::g     g:::::g l::::l o::::o     o::::ob:::::b    b::::::b    aaaaaaa:::::a  l::::l  s:::::s  ssssss      e:::::::eeeee::::::e  n:::::nnnn:::::nd::::::d    d:::::d 
g:::::g     g:::::g l::::l o::::o     o::::ob:::::b     b:::::b  aa::::::::::::a  l::::l    s::::::s           e:::::::::::::::::e   n::::n    n::::nd:::::d     d:::::d 
g:::::g     g:::::g l::::l o::::o     o::::ob:::::b     b:::::b a::::aaaa::::::a  l::::l       s::::::s        e::::::eeeeeeeeeee    n::::n    n::::nd:::::d     d:::::d 
g::::::g    g:::::g l::::l o::::o     o::::ob:::::b     b:::::ba::::a    a:::::a  l::::l ssssss   s:::::s      e:::::::e             n::::n    n::::nd:::::d     d:::::d 
g:::::::ggggg:::::gl::::::lo:::::ooooo:::::ob:::::bbbbbb::::::ba::::a    a:::::a l::::::ls:::::ssss::::::s     e::::::::e            n::::n    n::::nd::::::ddddd::::::dd
 g::::::::::::::::gl::::::lo:::::::::::::::ob::::::::::::::::b a:::::aaaa::::::a l::::::ls::::::::::::::s       e::::::::eeeeeeee    n::::n    n::::n d:::::::::::::::::d
  gg::::::::::::::gl::::::l oo:::::::::::oo b:::::::::::::::b   a::::::::::aa:::al::::::l s:::::::::::ss         ee:::::::::::::e    n::::n    n::::n  d:::::::::ddd::::d
    gggggggg::::::gllllllll   ooooooooooo   bbbbbbbbbbbbbbbb     aaaaaaaaaa  aaaallllllll  sssssssssss             eeeeeeeeeeeeee    nnnnnn    nnnnnn   ddddddddd   ddddd
            g:::::g                                                                                                                                                      
gggggg      g:::::g                                                                                                                                                      
g:::::gg   gg:::::g                                                                                                                                                      
 g::::::ggg:::::::g                                                                                                                                                      
  gg:::::::::::::g                                                                                                                                                       
    ggg::::::ggg                                                                                                                                                         
       gggggg                                                                                                                                                                                                                                  
*/








void recreateSwapChain();

void cleanup();

void createVertexBuffer();

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice,&physicalDeviceMemoryProperties);
	// = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	EXIT_CLEAN("failed to find suitable memory type!");
}


//vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory
void createBuffer(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer *buffer,
	VkDeviceMemory *bufferMemory){

	/*
	vk::BufferCreateInfo bufferInfo{ .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive };
    buffer = vk::raii::Buffer(device, bufferInfo);
    vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();
    vk::MemoryAllocateInfo allocInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties) };
    bufferMemory = vk::raii::DeviceMemory(device, allocInfo);
    buffer.bindMemory(*bufferMemory, 0);
	*/

	VkBufferCreateInfo bufferCreateInfo = {

		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		// .sharingMode = VK_SHARING_MODE_CONCURRENT,
		// .pQueueFamilyIndices = queueFamilyIndeces,
		// .queueFamilyIndexCount = queueFamilyIndexCount,
		
	};
	 
	vkCreateBuffer(device, &bufferCreateInfo, NULL, buffer);

	VkMemoryRequirements memoryRequirements={
		
	};
	
	vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {

		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits,properties),
		
	};

	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties ;

	vkGetPhysicalDeviceMemoryProperties(physicalDevice,&physicalDeviceMemoryProperties);

	vkAllocateMemory(device, &memoryAllocateInfo, NULL, bufferMemory);

	vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
}
void copyBuffer(VkBuffer  srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

	PRINT_FNAME;

	VkCommandBufferAllocateInfo allocInfo = { 
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = transferCommnadPool, 
		.level =  VK_COMMAND_BUFFER_LEVEL_PRIMARY, 
		.commandBufferCount = 1 
	};
    VkCommandBuffer commandCopyBuffer ;

	vkAllocateCommandBuffers(
		device, 
		&allocInfo, 
		&commandCopyBuffer
	);
	
	// = std::move(device.allocateCommandBuffers(allocInfo).front());


	VkCommandBufferBeginInfo pBeginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	vkBeginCommandBuffer(commandCopyBuffer, &pBeginInfo);
	VkBufferCopy bufferCopy = {
		.size = size,
		.dstOffset = 0,
		.srcOffset = 0
	};
	vkCmdCopyBuffer(commandCopyBuffer,  srcBuffer, dstBuffer, 1, &bufferCopy);
	vkEndCommandBuffer(commandCopyBuffer);


	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &commandCopyBuffer,
		
	};

	//no fences?
	vkQueueSubmit(transferQueue, 1, &submitInfo,  NULL);

	vkQueueWaitIdle(transferQueue);
}

void createVertexStagingBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices);

    VkBufferCreateInfo stagingInfo = { 
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = bufferSize, 
		.usage =VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE 
	};

	VkBuffer stagingBuffer;
    // VkBuffer stagingBuffer(de, stagingInfo);
	vkCreateBuffer(device, &stagingInfo, NULL, &stagingBuffer);

    VkMemoryRequirements memRequirementsStaging;
	vkGetBufferMemoryRequirements(device, stagingBuffer, &memRequirementsStaging);

	
    VkMemoryAllocateInfo memoryAllocateInfoStaging = 
	{  
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirementsStaging.size, 
		.memoryTypeIndex = findMemoryType(memRequirementsStaging.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) 
	};

    // VkDeviceMemory stagingBufferMemory(device, memoryAllocateInfoStaging);

	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties ;

	vkGetPhysicalDeviceMemoryProperties(physicalDevice,&physicalDeviceMemoryProperties);

	VkDeviceMemory bufferMemory;

	vkAllocateMemory(device, &memoryAllocateInfoStaging, NULL, &bufferMemory);

	vkBindBufferMemory(device, stagingBuffer, bufferMemory, 0);


	{
			void * data = NULL;

			vkMapMemory( device, bufferMemory, 0, bufferSize, 0, &data);

			memcpy(data, vertices, sizeof(vertices));

			VkMappedMemoryRange mappedMemoryRange = {
				.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
				.memory = bufferMemory,
				.offset = 0,
				.size = bufferSize,
				
			};

			vkFlushMappedMemoryRanges(device, 1, &mappedMemoryRange);

			vkUnmapMemory(device, bufferMemory);
	}


    // stagingBuffer.bindMemory(stagingBufferMemory, 0);
    // void* dataStaging = stagingBufferMemory.mapMemory(0, stagingInfo.size);
    // memcpy(dataStaging, vertices.data(), stagingInfo.size);
    // stagingBufferMemory.unmapMemory();

	 VkBufferCreateInfo bufferInfo = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = bufferSize,
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,

	};
	
	vkCreateBuffer(
		device, 
		&bufferInfo,
		NULL,
		&vertextBuffer
	);

    // VkBufferCreateInfo bufferInfo{ .size = bufferSize,  .usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, .sharingMode = vk::SharingMode::eExclusive };
    // vertexBuffer = vk::raii::Buffer(device, bufferInfo);

	VkMemoryRequirements memRequirements;

	vkGetBufferMemoryRequirements(
		device, 
		vertextBuffer, 
		&memRequirements
	);

	VkMemoryAllocateInfo memoryAllocateInfo = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
	};

	vkAllocateMemory(device, &memoryAllocateInfo, NULL, &deviceMemory);

    // vk::MemoryRequirements memRequirements = vertexBuffer.getMemoryRequirements();
    // vk::MemoryAllocateInfo memoryAllocateInfo{  .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal) };
    // vertexBufferMemory = vk::raii::DeviceMemory( device, memoryAllocateInfo );

    // vertexBuffer.bindMemory( *vertexBufferMemory, 0 );

	vkBindBufferMemory(device, vertextBuffer, deviceMemory, 0);

    copyBuffer(stagingBuffer, vertextBuffer,  bufferSize);



	vkDestroyBuffer(device, stagingBuffer, NULL);

	vkFreeMemory(device, bufferMemory, NULL);
}

void createVertexBuffer(){

	VkDeviceSize bufferSize = sizeof(vertices);

	/**

	Change the sharingMode of resources to be VK_SHARING_MODE_CONCURRENT and specify both the graphics and transfer queue families


	.usage = vk::BufferUsageFlagBits::eVertexBuffer, .sharingMode = vk::SharingMode::eExclusive
	
	*/
	// VkBufferCreateInfo bufferCreateInfo = {
	// 	.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	// 	.size = sizeof(vertices),
	// 	.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	// 	// .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	// 	.sharingMode = VK_SHARING_MODE_CONCURRENT,
	// 	.pQueueFamilyIndices = queueFamilyIndeces,
	// 	.queueFamilyIndexCount = queueFamilyIndexCount,
		
	// };

	// vkCreateBuffer(device, &bufferCreateInfo, NULL, &vertextBuffer);


	// VkMemoryRequirements memoryRequirements;
	

	// vkGetBufferMemoryRequirements(device, vertextBuffer, &memoryRequirements);
	

	// VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties ;

	// vkGetPhysicalDeviceMemoryProperties(physicalDevice,&physicalDeviceMemoryProperties);

	
	
	// /**
	//     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT = 0x00000002,
    // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT = 0x00000004,
	// */

	// printf("\nmemoryRequirements.memoryTypeBits: %x\n",memoryRequirements.memoryTypeBits);


	// VkMemoryPropertyFlags memoryProperyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	
	// int memoryTypeIndex = -1;

	// for(int i=0;i<physicalDeviceMemoryProperties.memoryTypeCount;i++)
	// {
	// 	printf("physicalDeviceMemoryProperties.memoryTypes[%d].propertyFlags: %x\n",i,physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags);
		
	// 	if((memoryRequirements.memoryTypeBits & (1 << i)) && 
	// 	(physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryProperyFlags) == memoryProperyFlags){
	// 		memoryTypeIndex = i;
	// 		break;
	// 	}
	// }
	// if(memoryTypeIndex == -1)
	// {
	// 	EXIT_CLEAN("failed to find suitable memory type!");
	// }
	
	// VkMemoryAllocateInfo memoryAllocateInfo = {
	// 	.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	// 	.memoryTypeIndex = memoryTypeIndex,
	// 	.allocationSize = memoryRequirements.size,
		
		
	// };
	// vkAllocateMemory(device, &memoryAllocateInfo, NULL, &deviceMemory);
	

	// vkBindBufferMemory(device, vertextBuffer, deviceMemory, 0);


	createBuffer(
		bufferSize, 
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		&vertextBuffer, 
		&deviceMemory
	);


	void * data = NULL;

	vkMapMemory( device, deviceMemory, 0, bufferSize, 0, &data);

	memcpy(data, vertices, sizeof(vertices));

	VkMappedMemoryRange mappedMemoryRange = {
		.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
		.memory = deviceMemory,
		.offset = 0,
		.size = bufferSize,
		
	};

	vkFlushMappedMemoryRanges(device, 1, &mappedMemoryRange);

	vkUnmapMemory(device, deviceMemory);
	
}

static void framebufferResizeCallback(GLFWwindow *win,int w,int h)
{
	framebufferResized = true;
}


void createSurface(){
	PRINT_FNAME;;

	glfwCreateWindowSurface(instance, window,NULL, &surface);
	
	
}

uint32_t createShaderFromFile(char * path, uint8_t** buffer){

	PRINT_FNAME;

	FILE *file = fopen(path, "rb");  // open file in read mode

    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }


	fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    *buffer = malloc(size);
    if (*buffer == NULL) {
        printf("Fill buffer mem alloc error\n");
        fclose(file);
        return 0;
    }

    // read file
    fread(*buffer, 1, size, file);


	// free(buffer);
    fclose(file);  // close file

	return size;
}
bool isDeviceSuitable(VkPhysicalDevice device)
{
	PRINT_FNAME;;
 
	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	VkPhysicalDeviceProperties physicalDeviceProperties;


	vkGetPhysicalDeviceFeatures(device,&physicalDeviceFeatures);
	vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);


    if (physicalDeviceProperties.deviceType > VK_PHYSICAL_DEVICE_TYPE_OTHER && 
		physicalDeviceFeatures.geometryShader) {
        return true;
    }

    return false;
}
void createCommandPool(){
	PRINT_FNAME;

	VkCommandPoolCreateInfo commandPoolCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = queueFamilyIndeces[graphicsQueueFamilyArrayIndex],
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	
	vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &graphicsCommnadPool);

	VkCommandBufferAllocateInfo allocInfo = { 
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool =  graphicsCommnadPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = MAX_FRAMES_IN_FLIGHT  ,
	};

	vkAllocateCommandBuffers(device, &allocInfo, graphicsCommandBuffers);


	// --
	commandPoolCreateInfo = (VkCommandPoolCreateInfo){
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = queueFamilyIndeces[transferQueueFamilyArrayIndex] ,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	
	vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &transferCommnadPool);

	 allocInfo = (VkCommandBufferAllocateInfo){ 
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool =  transferCommnadPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = transferCommandBuffersCount  ,
		
	};

	vkAllocateCommandBuffers(device, &allocInfo, &transferCommandBuffers);

	

}

void recordCommandBuffer(uint32_t imageIndex,uint32_t frameIndex) {
	// PRINT_FNAME;;

	/**
	
	commandBuffer.begin({});

    // Transition the image layout for rendering
    transition_image_layout(
        imageIndex,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal,
        {},
        vk::AccessFlagBits2::eColorAttachmentWrite,
        vk::PipelineStageFlagBits2::eColorAttachmentOutput,
        vk::PipelineStageFlagBits2::eColorAttachmentOutput
    );

    // Set up the color attachment
    vk::ClearValue clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
    vk::RenderingAttachmentInfo attachmentInfo = {
        .imageView = swapChainImageViews[imageIndex],
        .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
        .loadOp = vk::AttachmentLoadOp::eClear,
        .storeOp = vk::AttachmentStoreOp::eStore,
        .clearValue = clearColor
    };

    // Set up the rendering info
    vk::RenderingInfo renderingInfo = {
        .renderArea = { .offset = { 0, 0 }, .extent = swapChainExtent },
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachmentInfo
    };

    // Begin rendering
    commandBuffer.beginRendering(renderingInfo);

    // Rendering commands will go here

    // End rendering
    commandBuffer.endRendering();

    // Transition the image layout for presentation
    transition_image_layout(
        imageIndex,
        vk::ImageLayout::eColorAttachmentOptimal,
        vk::ImageLayout::ePresentSrcKHR,
        vk::AccessFlagBits2::eColorAttachmentWrite,
        {},
        vk::PipelineStageFlagBits2::eColorAttachmentOutput,
        vk::PipelineStageFlagBits2::eBottomOfPipe
    );

    commandBuffer.end();
	
	*/

/*
.flags 
	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: The command buffer will be rerecorded right after executing it once.

	VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT: This is a secondary command buffer that will be entirely within a single render pass.

	VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: The command buffer can be resubmitted while it is also already pending execution.

*/
	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pInheritanceInfo = NULL,	
	};

	vkBeginCommandBuffer(graphicsCommandBuffers[frameIndex], &beginInfo );


	/**
	 transition_image_layout(
        imageIndex,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal,
        {},
        vk::AccessFlagBits2::eColorAttachmentWrite,
        vk::PipelineStageFlagBits2::eColorAttachmentOutput,
        vk::PipelineStageFlagBits2::eColorAttachmentOutput
    );

	    uint32_t imageIndex,
    vk::ImageLayout oldLayout,
    vk::ImageLayout newLayout,
    vk::AccessFlags2 srcAccessMask,
    vk::AccessFlags2 dstAccessMask,
    vk::PipelineStageFlags2 srcStageMask,
    vk::PipelineStageFlags2 dstStageMask

	 vk::ImageMemoryBarrier2 barrier = {
        .srcStageMask = srcStageMask,
        .srcAccessMask = srcAccessMask,
        .dstStageMask = dstStageMask,
        .dstAccessMask = dstAccessMask,
        .oldLayout = oldLayout,
        .newLayout = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = swapChainImages[imageIndex],
        .subresourceRange = {
            .aspectMask = vk::ImageAspectFlagBits::eColor,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };
	
	*/
	VkImageMemoryBarrier2 imageMemoryBarrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.srcAccessMask = 0,
		.dstAccessMask= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		
		.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,

		.image = swapchainImages[imageIndex],

		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,

		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
		},
	};

	 VkDependencyInfo dependencyInfo = {
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .dependencyFlags = {},
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &imageMemoryBarrier
    };

	vkCmdPipelineBarrier2(graphicsCommandBuffers[frameIndex], &dependencyInfo);


	VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

    VkRenderingAttachmentInfo attachmentInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = swapchainImageViews[imageIndex],
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp =VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = clearColor
    };

    // Set up the rendering info
    VkRenderingInfo renderingInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = { 
			.offset = { 0, 0 }, 
			//.extent = surfaceCapabilities.currentExtent,
			.extent = imageExtent,
		},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachmentInfo
    };

	vkCmdBeginRendering(graphicsCommandBuffers[frameIndex], &renderingInfo);
    // Begin rendering
    

	vkCmdBindPipeline(graphicsCommandBuffers[frameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);


	VkViewport viewPort = {
		.x = 0,
		.y = 0,
		// .width = surfaceCapabilities.currentExtent.width,
		// .height = surfaceCapabilities.currentExtent.height,

		.width = imageExtent.width,
		.height = imageExtent.height,
		
	};
	vkCmdSetViewport(graphicsCommandBuffers[frameIndex], 0, 1, &viewPort);

	VkRect2D scissor = {
		.extent = imageExtent,
			// .extent = surfaceCapabilities.currentExtent,
		.offset = {},
	};

	vkCmdSetScissor(graphicsCommandBuffers[frameIndex], 0, 1, &scissor);



	vkCmdBindPipeline(graphicsCommandBuffers[frameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkDeviceSize offset = 0;

	vkCmdBindVertexBuffers(graphicsCommandBuffers[frameIndex], 0, 1, &vertextBuffer, &offset);

// 	commandBuffers[frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline);

// commandBuffers[frameIndex].bindVertexBuffers(0, *vertexBuffer, {0});


	vkCmdDraw(graphicsCommandBuffers[frameIndex], 3, 1, 0, 0);

	vkCmdEndRendering(graphicsCommandBuffers[frameIndex]);

	/**
	transition_image_layout(
    imageIndex,
    vk::ImageLayout::eColorAttachmentOptimal,
    vk::ImageLayout::ePresentSrcKHR,
    vk::AccessFlagBits2::eColorAttachmentWrite,             // srcAccessMask
    {},                                                     // dstAccessMask
    vk::PipelineStageFlagBits2::eColorAttachmentOutput,     // srcStage
    vk::PipelineStageFlagBits2::eBottomOfPipe               // dstStage
);
	
	*/

	 imageMemoryBarrier = (VkImageMemoryBarrier2){
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,

		.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		.dstAccessMask= {},
		
		.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,

		.image = swapchainImages[imageIndex],

		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,

		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
		},
	};


	 dependencyInfo = (VkDependencyInfo){
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .dependencyFlags = {},
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &imageMemoryBarrier
    };

	vkCmdPipelineBarrier2(graphicsCommandBuffers[frameIndex], &dependencyInfo);


	vkEndCommandBuffer(graphicsCommandBuffers[frameIndex]);

}


void drawFrame() {
	// PRINT_FNAME;;

	/**
	
	Wait for the previous frame to finish

	Acquire an image from the swap chain

	Record a command buffer which draws the scene onto that image

	Submit the recorded command buffer

	Present the swap chain image
	
	*/


	VkResult result = vkWaitForFences(
		device, 
		1, 
		&inFlightFences[frameIndex],
		VK_TRUE, UINT64_MAX
	);

	if(result != VK_SUCCESS)
	{
		EXIT_CLEAN("failed to wait for fence!");
	}

	uint32_t imageIndex =  -1;
	result = vkAcquireNextImageKHR(
		device, 
		swapchain, 
		UINT64_MAX, 
		presentCompleteSemaphore[frameIndex], 
		NULL, 
		&imageIndex
	);

	if(result == VK_ERROR_OUT_OF_DATE_KHR){
		recreateSwapChain();
		return;
	}else if(result != VK_SUCCESS && result!= VK_SUBOPTIMAL_KHR)
	{
		assert(result == VK_TIMEOUT || result == VK_NOT_READY);
		// printf("failed to acquire swap chain image!\n");
		EXIT_CLEAN("failed to acquire swap chain image!");
	}

	

	vkResetFences(device, 1, &inFlightFences[frameIndex]);
	// auto fenceResult = device.waitForFences(*drawFence, vk::True, UINT64_MAX);

	recordCommandBuffer(imageIndex,frameIndex);

	VkPipelineStageFlags waitDestinationStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ;

	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &presentCompleteSemaphore[frameIndex],
		
		.pWaitDstStageMask = &waitDestinationStageMask,
		
		.commandBufferCount = 1,
		.pCommandBuffers = &graphicsCommandBuffers[frameIndex],

		.signalSemaphoreCount = 1,
		.pSignalSemaphores =  &renderFinishedSemaphore[frameIndex],

	};

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[frameIndex]);


	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &renderFinishedSemaphore[frameIndex],
		.swapchainCount = 1,
		.pSwapchains = &swapchain,
		.pImageIndices = &imageIndex,
		
	};

	// assert(false);

	result = vkQueuePresentKHR(graphicsQueue, &presentInfo);

	if(
		(result == VK_SUBOPTIMAL_KHR) || 
		(result == VK_ERROR_OUT_OF_DATE_KHR) || 
		framebufferResized
	){
		framebufferResized = false;
		recreateSwapChain();
	}else
	{
		assert(result == VK_SUCCESS);
	}
	frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;

}
void createSyncObjects(){
	
	PRINT_FNAME;;



	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{

	
		VkSemaphoreCreateInfo semaphoreCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};
		vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &presentCompleteSemaphore[i]);

		vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &renderFinishedSemaphore[i]);

		VkFenceCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT,
		};

		vkCreateFence(device, &createInfo, NULL,  &inFlightFences[i]);
	}
	
    
}

void queueFamilyCheck(){

	PRINT_FNAME;
	

	uint32_t queueFamilyPropertieCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertieCount, NULL);
	VkQueueFamilyProperties queueFamilyProperties[queueFamilyPropertieCount];
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertieCount, queueFamilyProperties);
	
	/**
	VK_QUEUE_GRAPHICS_BIT = 0x00000001,
    VK_QUEUE_COMPUTE_BIT = 0x00000002,
    VK_QUEUE_TRANSFER_BIT = 0x00000004,
    VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
    VK_QUEUE_PROTECTED_BIT = 0x00000010,
    VK_QUEUE_VIDEO_DECODE_BIT_KHR = 0x00000020,
    VK_QUEUE_VIDEO_ENCODE_BIT_KHR = 0x00000040,
    VK_QUEUE_OPTICAL_FLOW_BIT_NV = 0x00000100,
	
	*/

	queueFamilyIndeces[graphicsQueueFamilyArrayIndex] = -1;
	  

	for(int i=0;i<queueFamilyPropertieCount;i++){
		printf("queue count: %d, flags: %x\n",queueFamilyProperties[i].queueCount,queueFamilyProperties[i].queueFlags);
		
		if(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
			queueFamilyIndeces[graphicsQueueFamilyArrayIndex]  = i;
			break;
		}
	}
	if(queueFamilyIndeces[graphicsQueueFamilyArrayIndex]  == - 1 ){
		EXIT_CLEAN("No supported VK_QUEUE_GRAPHICS_BIT");
	}

	queueFamilyIndeces[transferQueueFamilyArrayIndex] = -1;
	
	for(int i=0;i<queueFamilyPropertieCount;i++){
		printf("queue count: %d, flags: %x\n",queueFamilyProperties[i].queueCount,queueFamilyProperties[i].queueFlags);
		
		if(queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT){
			queueFamilyIndeces[transferQueueFamilyArrayIndex]  = i;
			break;
		}
	}

	if(queueFamilyIndeces[transferQueueFamilyArrayIndex]  == -1){
		EXIT_CLEAN("No supported VK_QUEUE_TRANSFER_BIT");
	}


}
void physicalDeviceExtensionCheck(){

	PRINT_FNAME;


	uint32_t deviceExtensionPropertieCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, NULL,&deviceExtensionPropertieCount,NULL);

	VkExtensionProperties exp_props[deviceExtensionPropertieCount];
	vkEnumerateDeviceExtensionProperties(physicalDevice, NULL,&deviceExtensionPropertieCount,exp_props);

	// bool found = false;
	int supportedCnt = 0;
	// bool requiredDeviceExtensionSupport = false;
	for(int i=0;i<deviceExtensionPropertieCount; i++){

		// printf("\tphys device extension: %s\n",exp_props[i].extensionName);

		for(int k = 0;k < requiredDeviceExtensionCount ; k++)
		{
			if(strcmp(exp_props[i].extensionName, requiredDeviceExtensions[k]) == 0){
				supportedCnt ++;
				break;
			}
		}
	}
	
	// if(supportedCnt == requiredDeviceExtensionCount)
	// {
		 
	// 	requiredDeviceExtensionSupport = true;
	// }

	if(!(supportedCnt == requiredDeviceExtensionCount)){
		printf("No suported extensions\n");
		printf("supportedCnt: %d of %d\n",supportedCnt, requiredDeviceExtensionCount);

		for(int i=0;i<deviceExtensionPropertieCount; i++){

			printf("\tfound: %s\n",exp_props[i].extensionName);
		}
		for(int i=0;i<requiredDeviceExtensionCount; i++){

			printf("\trequired: %s\n",requiredDeviceExtensions[i]);
		}
		// exit(1);
		EXIT_CLEAN("\n");
	}

}

void physicalDeviceFeatureCheck(){

	PRINT_FNAME;

	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2={
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
	};

	// VkPhysicalDeviceExtendedDynamicStateFeaturesEXT dynamic = {
	// 	.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
	// };

	VkPhysicalDeviceVulkan13Features features13 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.dynamicRendering = VK_TRUE,
		.synchronization2 = VK_TRUE,
	};

	physicalDeviceFeatures2.pNext = &features13;
	// dynamic.pNext = &features13;

	vkGetPhysicalDeviceFeatures2(physicalDevice, &physicalDeviceFeatures2 );

	VkPhysicalDeviceFeatures2 * next = &physicalDeviceFeatures2;
	int all_ok = 0;
	// requiredDeviceFeaturesSupport = false;
	while(next!= NULL){
	
		switch(next->sType){
			case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:
					printf("supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2\n");
					all_ok ++;
				break;
			case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT:
					printf("supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT\n");
					all_ok ++;
				break;
			case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES:
					printf("supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES\n");
					all_ok ++;
				break;
			default:
					printf("next %d\n",next->sType);
				break;
		}		
		next = next->pNext;
	}
	if(all_ok != 2){
		printf("supported :%d \n",all_ok);
		EXIT_CLEAN("Some Device features not supported");
	}
	// requiredDeviceFeaturesSupport = true;
}
void  createLogicalDevice(){

	PRINT_FNAME;

	uint32_t physicalDeviceQueueFamilyPropertieCount = 0 ;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamilyPropertieCount, NULL);

	VkQueueFamilyProperties queueFamilyProperties[physicalDeviceQueueFamilyPropertieCount];

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamilyPropertieCount, queueFamilyProperties);


	// bothQueueFamilyIndex = -1;
	
	// for(int index = 0; index<physicalDeviceQueueFamilyPropertieCount; index++ ){
	// 	VkBool32 supported = VK_FALSE;
	// 	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &supported);

	// 	if((queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) && supported){
	// 		bothQueueFamilyIndex = index;
	// 		break;
	// 	}
	// }

	VkBool32 supported = VK_FALSE;
	
	presentationSupportQueueFamilyIndex = - 1;

	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndeces[graphicsQueueFamilyArrayIndex] , surface, &supported);

	if(supported == VK_TRUE)
	{
		presentationSupportQueueFamilyIndex = queueFamilyIndeces[graphicsQueueFamilyArrayIndex];
	}
	else
	{
		EXIT_CLEAN("vkGetPhysicalDeviceSurfaceSupportKHR == false");
	}


	float queuePriority = 0.5f;
	VkDeviceQueueCreateInfo deviceQueueCreateInfo={
		.sType =  VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = queueFamilyIndeces[graphicsQueueFamilyArrayIndex],
		.queueCount = 1,
		.pQueuePriorities = &queuePriority,
	};

	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2={
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2

	};
	VkPhysicalDeviceVulkan13Features physicalDeviceFeatures13={
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.dynamicRendering = VK_TRUE,
		.synchronization2 = VK_TRUE


	};
	// VkPhysicalDeviceExtendedDynamicStateFeaturesEXT physicalDeviceExtendedDynamicStateFeaturesEXT={
	// 	.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
	// 	.extendedDynamicState = VK_TRUE
	// };

	
	physicalDeviceFeatures2.pNext = &physicalDeviceFeatures13;
	// physicalDeviceFeatures13.pNext = &physicalDeviceExtendedDynamicStateFeaturesEXT;
	

	VkDeviceCreateInfo deviceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = &deviceQueueCreateInfo,
		.queueCreateInfoCount = 1,
		.pNext = &physicalDeviceFeatures2,
		.ppEnabledExtensionNames = requiredDeviceExtensions,
		.enabledExtensionCount = requiredDeviceExtensionCount,
	};

	VkResult res = vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device);
	if(res != VK_SUCCESS)
	{
	
		EXIT_CLEAN("failed to create logical device");
	}

	
	
	exit:
	
}

void createPhysicalDevice()
{
	PRINT_FNAME;
	
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL);

	VkPhysicalDevice physicalDevices[physicalDeviceCount];

	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);




	VkPhysicalDeviceFeatures features;

	VkPhysicalDeviceProperties properties;

	
	uint32_t ratings[physicalDeviceCount]={};

	for(int i=0;i<physicalDeviceCount;i++){

		 
		vkGetPhysicalDeviceFeatures(physicalDevices[i],&features);
		vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);
		printf("\ndevice name: %s\n",properties.deviceName);

				/*
		    VK_PHYSICAL_DEVICE_TYPE_OTHER = 0,
			VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU = 1,
			VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU = 2,
			VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU = 3,
			VK_PHYSICAL_DEVICE_TYPE_CPU = 4,
		
		*/

		if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
		{

			ratings[i] +=1000;
		}
		else if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU )
		{
		 
			ratings[i] +=500;
			
		}

		ratings[i]  += properties.limits.maxImageDimension2D;

    	if (!features.geometryShader)
		{
			ratings[i] = 0;
			continue;	
        }
		
		
	}
	uint32_t max = 1;
	uint32_t max_id = 1000;
	for(int i=0;i<physicalDeviceCount;i++){
		if(max < ratings[i]){
			max_id = i;
			max = ratings[i];
		}
	}
	if(max_id == 1000)
	{
		printf("failed to find a suitable GPU!\n");
		exit(1);
	}
	physicalDevice = physicalDevices[max_id];


	
	vkGetPhysicalDeviceFeatures(physicalDevice,&features);
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	printf("\nSELECTED device name: %s\n",properties.deviceName);
	printf("device rating: %d\n",ratings[max_id]);
	printf("device type: %d\n",properties.deviceType);
	printf("geometry shader: %d\n",features.geometryShader);
	bool supportsVulkan1_3 = properties.apiVersion >= VK_API_VERSION_1_3;
	printf("API >= 1.3 support: %d\n",supportsVulkan1_3);
	printf("\n");



}

void createSwapchain(){
	PRINT_FNAME;
	
	
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		physicalDevice, 
		surface, 
		&surfaceCapabilities);


	printf(
		"Surface extent: w:%d h:%d\n",
		surfaceCapabilities.currentExtent.width, 
		surfaceCapabilities.currentExtent.height
	);

	uint32_t physicalDeviceSurfaceFormatCount = 0;

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		physicalDevice, 
		surface, 
		&physicalDeviceSurfaceFormatCount , NULL);

	VkSurfaceFormatKHR surfaceFormats[physicalDeviceSurfaceFormatCount];

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		physicalDevice, surface, 
		&physicalDeviceSurfaceFormatCount , surfaceFormats);

	printf("Surface formats:\n");

	
	swapchainSurfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;

	swapchainSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	// uint32_t color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	
	
	for(int i=0;i<physicalDeviceSurfaceFormatCount;i++)
	{
		printf("\tSurfaceFormat.format: %d\n",surfaceFormats[i].format);

		swapchainSurfaceFormat = surfaceFormats[i].format;

		if(swapchainSurfaceFormat == VK_FORMAT_B8G8R8A8_SRGB)
		{
			break;
		}
	}

	for(int i=0;i<physicalDeviceSurfaceFormatCount;i++)
	{
		printf("\tSurfaceFormat.colorSpace:%d\n",surfaceFormats[i].colorSpace);

		swapchainSurfaceColorSpace = surfaceFormats[i].colorSpace;

		if(swapchainSurfaceColorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			break;
		}
		
	}

	uint32_t presentModeCount = 0 ;

	vkGetPhysicalDeviceSurfacePresentModesKHR(
		physicalDevice,
		surface, 
		&presentModeCount, NULL);

	VkPresentModeKHR presentModes[presentModeCount];
	
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		physicalDevice, 
		surface, 
		&presentModeCount, 
		presentModes);
	


	printf("Surface pPresentModes:\n");

	uint32_t presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

	for(int i=0;i<presentModeCount;i++)
	{
		printf("\tpresent mode: %d\n",presentModes[i]);

		if(presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			presentMode  = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}



	/**
	
	vk::SwapchainCreateInfoKHR swapChainCreateInfo{.surface          = *surface,
                                               .minImageCount    = minImageCount,
                                               .imageFormat      = swapChainSurfaceFormat.format,
                                               .imageColorSpace  = swapChainSurfaceFormat.colorSpace,
                                               .imageExtent      = swapChainExtent,
                                               .imageArrayLayers = 1,
                                               .imageUsage       = vk::ImageUsageFlagBits::eColorAttachment,
                                               .imageSharingMode = vk::SharingMode::eExclusive,
                                               .preTransform     = surfaceCapabilities.currentTransform,
                                               .compositeAlpha   = vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                               .presentMode      = chooseSwapPresentMode(availablePresentModes),
                                               .clipped          = true};
};


typedef enum VkCompositeAlphaFlagBitsKHR {
    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR = 0x00000001,
    VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR = 0x00000002,
    VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR = 0x00000004,
    VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR = 0x00000008,
    VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR = 0x7FFFFFFF
} VkCompositeAlphaFlagBitsKHR;
	*/

	imageExtent = surfaceCapabilities.currentExtent;

	if(imageExtent.width > surfaceCapabilities.maxImageExtent.width 
		&& imageExtent.height > surfaceCapabilities.maxImageExtent.height 
	){
		imageExtent = surfaceCapabilities.maxImageExtent;
		imageExtent = (VkExtent2D){
			.width = WIDTH,
			.height = HEIGHT,
		};
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	if(
		width >= surfaceCapabilities.minImageExtent.width &&
		width <= surfaceCapabilities.maxImageExtent.width &&
		height >= surfaceCapabilities.minImageExtent.height &&
		height <= surfaceCapabilities.maxImageExtent.height
		)
		{
			imageExtent = (VkExtent2D){
			.width = width,
			.height = height,
			};
		}
	
	VkSwapchainCreateInfoKHR createInfo={
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.minImageCount = surfaceCapabilities.minImageCount  == 0? 1: surfaceCapabilities.minImageCount,
		.imageFormat = swapchainSurfaceFormat,
		.imageColorSpace = swapchainSurfaceColorSpace,
		.imageExtent = imageExtent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = surfaceCapabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = presentMode,
		.surface = surface,
		.clipped = true,
	};
	
	
	
	vkCreateSwapchainKHR(device, &createInfo, NULL, &swapchain);

}



void createImageViews(){

	PRINT_FNAME;;

	vkGetSwapchainImagesKHR(device, swapchain,  &swapchainImageCount,NULL);

	vkGetSwapchainImagesKHR(device, swapchain,  &swapchainImageCount, swapchainImages);

	printf("pSwapchainImageCount %d\n",swapchainImageCount);
	
	for(int i=0;i<swapchainImageCount;i++)
	{

		printf("img: %p\n",swapchainImages[i]);
	}

	swapchainImageViewCount = swapchainImageCount;

	// vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t *pSurfaceFormatCount, VkSurfaceFormatKHR *pSurfaceFormats)

	/**
	
	{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 } 

	typedef struct VkImageSubresourceRange {
    VkImageAspectFlags    aspectMask;
    uint32_t              baseMipLevel;
    uint32_t              levelCount;
    uint32_t              baseArrayLayer;
    uint32_t              layerCount;
} VkImageSubresourceRange;

	*/

	
	VkImageViewCreateInfo imageViewCreateInfo ={
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = VK_FORMAT_B8G8R8A8_SRGB,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
		
	};

	for(int i=0;i<swapchainImageViewCount;i++)
	{
		imageViewCreateInfo.image = swapchainImages[i];
		vkCreateImageView(device, &imageViewCreateInfo,NULL, &swapchainImageViews[i]);
	
	}


	//  vk::ImageViewCreateInfo imageViewCreateInfo{ .viewType         = vk::ImageViewType::e2D,
    //                                              .format           = swapChainSurfaceFormat.format,
    //                                              .subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 } };
}
void cleanupSwapChain() {


	for(int i=0;i<swapchainImageViewCount;i++)
	{
		vkDestroyImageView(device, swapchainImageViews[i], NULL);
	}
	swapchainImageViewCount = 0;
	
    vkDestroySwapchainKHR(device, swapchain, NULL);

	swapchain = NULL;

}

void recreateSwapChain(){
	PRINT_FNAME;

	int width = 0,height = 0;

	glfwGetFramebufferSize(window, &width, &height);

	while(width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}


	vkDeviceWaitIdle(device);

	cleanupSwapChain();

	createSwapchain();	

	createImageViews();
	
}

VkBool32 debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData)
{
	
	printf("validation layer:%d %d %s\n",messageSeverity,messageTypes ,pCallbackData->pMessage);
	return VK_FALSE;
}




void setupDebugMessenger()
{
	PRINT_FNAME;;
    if (!enableValidationLayers) return;

	VkDebugUtilsMessageSeverityFlagsEXT severityFlags = 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ;

	VkDebugUtilsMessageTypeFlagsEXT messageTypeFlags =     
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT ;

	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = severityFlags,
		.messageType = messageTypeFlags,
		.pfnUserCallback = debugCallback
	};
	assert(instance);


	 PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
        if(func(instance, &debugUtilsMessengerCreateInfoEXT, NULL, &debugUtilsMessengerEXT)!= VK_SUCCESS){
			printf("%s\n","cannot setup debug messenger");
		}
    } else {
     

		printf("%s\n","VK_ERROR_EXTENSION_NOT_PRESENT");
	
    }
 

}
void createQueue(){
	
	vkGetDeviceQueue(device, queueFamilyIndeces[graphicsQueueFamilyArrayIndex], 0, &graphicsQueue);
	vkGetDeviceQueue(device, queueFamilyIndeces[transferQueueFamilyArrayIndex], 0, &transferQueue);
	
}

void createInstance(){
	printf( "%s\n", __FUNCTION__ );

	VkApplicationInfo applicationInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 ),
		.pEngineName        = "No Engine",
		.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 ),
		.apiVersion         = VK_API_VERSION_1_4
	};

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions  =  glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	

	
	uint32_t glfwExtensionCountExtra = glfwExtensionCount + 1;
	const char* glfwExtensionsExtra[glfwExtensionCountExtra];

	for(int i=0;i<glfwExtensionCount;i++){
			glfwExtensionsExtra[i] = glfwExtensions[i];
	}
	
	

	if(enableValidationLayers){
		
		glfwExtensionsExtra[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
	}else
	{
		glfwExtensionCountExtra = glfwExtensionCount;
	}

	
	// printf("glfwExtensionsExtra [%d]:\n",glfwExtensionCountExtra);
	for(int i=0;i<glfwExtensionCountExtra;i++){

		printf("extensions extra: %s\n",glfwExtensionsExtra[i]);
	}

	{

		uint32_t layerCount = 0;
		VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, NULL);
		if (result != VK_SUCCESS) {
			printf("Failed to get layer count\n");
			return ;
		}

		VkLayerProperties layers[layerCount];
		result = vkEnumerateInstanceLayerProperties(&layerCount, layers);
		if (result != VK_SUCCESS) {
			printf("Failed to enumerate layers\n");
		 
			return  ;
		}

		printf("Print layers:\n");
		bool validationLayerSupported = false;
		for (uint32_t i = 0; i < layerCount; i++) {
			
			printf("layer: %s\n",layers[i].layerName);

			uint32_t cnt = 0;

			vkEnumerateInstanceExtensionProperties(layers[i].layerName, &cnt, NULL);

			VkExtensionProperties expr[cnt];
			vkEnumerateInstanceExtensionProperties(layers[i].layerName, &cnt, expr);

			for(int i=0;i<cnt;i++){

				printf("\tlayer extensions: %s\n",expr[i].extensionName);
			}

			if(strcmp(layers[i].layerName, validationLayers[0])){
				validationLayerSupported = true;
			}
		}
		if(validationLayerSupported == false){
			printf("Required layer is not supported \n\t%s\n",validationLayers[0]);
			return ; 
		}

		 
	}
	 
	

	VkInstanceCreateInfo instanceCreateInfo={
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &applicationInfo,
		.enabledExtensionCount = glfwExtensionCountExtra,
		.ppEnabledExtensionNames = glfwExtensionsExtra,
		
	};
	

	if(enableValidationLayers)
	{
		instanceCreateInfo.enabledLayerCount = validationLayerCount;
		instanceCreateInfo.ppEnabledLayerNames = validationLayers;

	}


	if(vkCreateInstance(&instanceCreateInfo,NULL,&instance) != VK_SUCCESS ){
		// printf("vkCreateInstance failed\n");
		EXIT_CLEAN("vkCreateInstance failed");
	
	}


}
void createGraphicsPipeline() {

	PRINT_FNAME;

	uint8_t* data = NULL;
	size_t dataSize = createShaderFromFile("shaders/frag.spv", &data);
	
	//--- FRAGMENT



	if(dataSize == 0)
	{
		printf("failed to read %s\n","shaders/frag.spv");
		return;
	}
	VkShaderModuleCreateInfo createInfo={
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
		
	};
 
	vkCreateShaderModule(device, &createInfo, NULL, &shaderModuleFrag);

	free(data);



	//--- VERTEX
	
	dataSize = createShaderFromFile("shaders/vert.spv", &data);
	
	if(dataSize == 0)
	{
		printf("failed to read %s\n","shaders/vert.spv");
		return;
	}

	createInfo = (VkShaderModuleCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
		
	};
	
	vkCreateShaderModule(device, &createInfo, NULL, &shaderModuleVert);
	free(data);
	//---------

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = shaderModuleVert,
		.pName = "main",
		

	};

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = shaderModuleFrag,
		.pName = "main",

	};

	size_t dynamicStateCount = 2;
	VkDynamicState dynamicState[]={VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_SCISSOR};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = dynamicStateCount,
		.pDynamicStates = dynamicState,
		
	};

	
	VkPipelineShaderStageCreateInfo shaderStageCreateInf[] = {
		shaderStageCreateInfoVert,
		shaderStageCreateInfoFrag
	};

	size_t shaderStageCreateInfCnt = sizeof(shaderStageCreateInf) /  sizeof(VkPipelineShaderStageCreateInfo);



	VkVertexInputBindingDescription vertexInputBindingDescription  = {
		
		.binding = 0,
		.stride = sizeof(struct Vertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};
		/*
    uint32_t    location;
    uint32_t    binding;
    VkFormat    format;
    uint32_t    offset;	
	
	*/
	
	VkVertexInputAttributeDescription vertexInputAttributeDescriptions[2]={
		(VkVertexInputAttributeDescription){
			.location = 0,
			.binding = 0,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof(struct Vertex, pos),
		},
		(VkVertexInputAttributeDescription){
			.location = 1,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(struct Vertex, col),
		},
	};
	uint32_t vertexInputAttributeDescriptionsCount = sizeof(vertexInputAttributeDescriptions) / (sizeof(VkVertexInputAttributeDescription));



	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &vertexInputBindingDescription,
		.vertexAttributeDescriptionCount = vertexInputAttributeDescriptionsCount,
		.pVertexAttributeDescriptions = vertexInputAttributeDescriptions,
	
	};

	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	};

	VkViewport viewport = {
		.x = 0,
		.y = 0,
		// .height = surfaceCapabilities.currentExtent.height,
		// .width= surfaceCapabilities.currentExtent.width,
		.height = imageExtent.height,
		.width = imageExtent.width,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	//The actual viewport(s) and scissor rectangle(s) will then later be set up at drawing time.

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pViewports = {},
		.viewportCount = 1,
		.pScissors = {},
		.scissorCount  =1,
	};

 
	/**
	.depthClampEnable = vk::False, .rasterizerDiscardEnable = vk::False,
 .polygonMode = vk::PolygonMode::eFill, .cullMode = vk::CullModeFlagBits::eBack,
 .frontFace = vk::FrontFace::eClockwise, .depthBiasEnable = vk::False,
 .depthBiasSlopeFactor = 1.0f, .lineWidth = 1.0f 
	
	*/
	VkPipelineRasterizationStateCreateInfo rasterizer = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.depthBiasSlopeFactor = 1.0f,
		.lineWidth = 1.0f,
	};

	/*
	
	.rasterizationSamples = vk::SampleCountFlagBits::e1, .sampleShadingEnable = vk::False

	* */
	VkPipelineMultisampleStateCreateInfo multisampling = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
	};

	//for now?
	VkPipelineDepthStencilStateCreateInfo* depthStencil  = NULL;


	//Color blending

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState ={
		.blendEnable = VK_FALSE,
		.colorWriteMask  = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD,
	};

/**
.logicOpEnable = vk::False, .logicOp =  vk::LogicOp::eCopy, .attachmentCount = 1, .pAttachments =  &colorBlendAttachment 

*/
	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachmentState,
	};

	// (void)pipelineLayout;

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pushConstantRangeCount  = 0,
	};

	vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipelineLayout );

	/**
		 .colorAttachmentCount = 1, .pColorAttachmentFormats = &swapChainImageFormat 
	
	*/

	VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &swapchainSurfaceFormat,
	};
	
	/**
	
	.pNext = &pipelineRenderingCreateInfo,
    .stageCount = 2, .pStages = shaderStages,
    .pVertexInputState = &vertexInputInfo, .pInputAssemblyState = &inputAssembly,
    .pViewportState = &viewportState, .pRasterizationState = &rasterizer,
    .pMultisampleState = &multisampling, .pColorBlendState = &colorBlending,
    .pDynamicState = &dynamicState, .layout = pipelineLayout, .renderPass = nullptr 
	*/


	VkSubpassDependency subpassDependency = {
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = 0 ,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		// .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT ,
	};
	VkAttachmentDescription colorAttachment = {
		.format = swapchainSurfaceFormat,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};
	VkAttachmentReference colorRef = {
    	.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkSubpassDescription subpass = {
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorRef
	};

	VkRenderPassCreateInfo renderPassCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.dependencyCount = 1,
		.pDependencies = &subpassDependency, 
		.subpassCount = 1,
		.attachmentCount = 1,
		.pAttachments = &colorAttachment,
		.pSubpasses = &subpass,
		
	};

	vkCreateRenderPass(device, &renderPassCreateInfo, NULL, &renderPass);

	// renderpass


	VkGraphicsPipelineCreateInfo  graphicsPipelineCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO , 
		.pNext = &pipelineRenderingCreateInfo,
		.stageCount = shaderStageCreateInfCnt,
		.pStages = shaderStageCreateInf,
		.pVertexInputState = &pipelineVertexInputStateCreateInfo,
		.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo,
		.pViewportState = &viewportStateCreateInfo,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pColorBlendState = &colorBlendStateCreateInfo,
		.pDynamicState = &dynamicStateCreateInfo,
		.layout = pipelineLayout,
		// .renderPass = renderPass,
		.renderPass = VK_NULL_HANDLE,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = - 1,
		
		
		
	};

	vkCreateGraphicsPipelines(
		device,
		NULL, 
		1, 
		&graphicsPipelineCreateInfo,
		NULL, 
		&graphicsPipeline
	);

	/*
	VK_SUBPASS_EXTERNAL, 
	{},
	vk::PipelineStageFlagBits::eColorAttachmentOutput, 
	vk::PipelineStageFlagBits::eColorAttachmentOutput,
	{}, 
	vk::AccessFlagBits::eColorAttachmentWrite

	last: VK_DEPENDENCY_BY_REGION_BIT ?

	    uint32_t                srcSubpass;
    uint32_t                dstSubpass;
    VkPipelineStageFlags    srcStageMask;
    VkPipelineStageFlags    dstStageMask;
    VkAccessFlags           srcAccessMask;
    VkAccessFlags           dstAccessMask;
    VkDependencyFlags       dependencyFlags;

	*/



}





/**
                                                                                                                                                                    
                                                                                                                                                                    
IIIIIIIIII                  iiii          tttt                                                          lllllll kkkkkkkk                                            
I::::::::I                 i::::i      ttt:::t                                                          l:::::l k::::::k                                            
I::::::::I                  iiii       t:::::t                                                          l:::::l k::::::k                                            
II::::::II                             t:::::t                                                          l:::::l k::::::k                                            
  I::::Innnn  nnnnnnnn    iiiiiiittttttt:::::ttttttt         vvvvvvv           vvvvvvvuuuuuu    uuuuuu   l::::l  k:::::k    kkkkkkkaaaaaaaaaaaaa  nnnn  nnnnnnnn    
  I::::In:::nn::::::::nn  i:::::it:::::::::::::::::t          v:::::v         v:::::v u::::u    u::::u   l::::l  k:::::k   k:::::k a::::::::::::a n:::nn::::::::nn  
  I::::In::::::::::::::nn  i::::it:::::::::::::::::t           v:::::v       v:::::v  u::::u    u::::u   l::::l  k:::::k  k:::::k  aaaaaaaaa:::::an::::::::::::::nn 
  I::::Inn:::::::::::::::n i::::itttttt:::::::tttttt            v:::::v     v:::::v   u::::u    u::::u   l::::l  k:::::k k:::::k            a::::ann:::::::::::::::n
  I::::I  n:::::nnnn:::::n i::::i      t:::::t                   v:::::v   v:::::v    u::::u    u::::u   l::::l  k::::::k:::::k      aaaaaaa:::::a  n:::::nnnn:::::n
  I::::I  n::::n    n::::n i::::i      t:::::t                    v:::::v v:::::v     u::::u    u::::u   l::::l  k:::::::::::k     aa::::::::::::a  n::::n    n::::n
  I::::I  n::::n    n::::n i::::i      t:::::t                     v:::::v:::::v      u::::u    u::::u   l::::l  k:::::::::::k    a::::aaaa::::::a  n::::n    n::::n
  I::::I  n::::n    n::::n i::::i      t:::::t    tttttt            v:::::::::v       u:::::uuuu:::::u   l::::l  k::::::k:::::k  a::::a    a:::::a  n::::n    n::::n
II::::::IIn::::n    n::::ni::::::i     t::::::tttt:::::t             v:::::::v        u:::::::::::::::uul::::::lk::::::k k:::::k a::::a    a:::::a  n::::n    n::::n
I::::::::In::::n    n::::ni::::::i     tt::::::::::::::t              v:::::v          u:::::::::::::::ul::::::lk::::::k  k:::::ka:::::aaaa::::::a  n::::n    n::::n
I::::::::In::::n    n::::ni::::::i       tt:::::::::::tt               v:::v            uu::::::::uu:::ul::::::lk::::::k   k:::::ka::::::::::aa:::a n::::n    n::::n
IIIIIIIIIInnnnnn    nnnnnniiiiiiii         ttttttttttt                  vvv               uuuuuuuu  uuuullllllllkkkkkkkk    kkkkkkkaaaaaaaaaa  aaaa nnnnnn    nnnnnn
                                                                                                                                                                    
                                                                                                                                                                    
                                                                                                                                                                    
                                                                                                                                                                    
                                                                                                                                                                    
                                                                                                                                                                    
                                                                                                                                                                         
*/


























void initVulkan(){
	PRINT_FNAME;

	createInstance();

	setupDebugMessenger();

	createSurface();

	createPhysicalDevice();

	queueFamilyCheck();

	physicalDeviceExtensionCheck();

	physicalDeviceFeatureCheck();

	createLogicalDevice();
	
	createSwapchain();

	createImageViews();

	createQueue();

	createGraphicsPipeline();

	createCommandPool();

	createVertexStagingBuffer();

	// createVertexBuffer();

	

	createSyncObjects();

}
void mainLoop(){
	PRINT_FNAME;

	while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
		drawFrame();
    }

	vkDeviceWaitIdle(device);
	
}
void cleanup(){
	PRINT_FNAME;

	if(enableValidationLayers)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != NULL) {
			func(instance, debugUtilsMessengerEXT, NULL);
		}
	}
	/*

	order of freeing memory:
	
	3		VkInstance

	2	VkDevice

	1 Buffers / Images

	1 Pipelines

	1 Descriptor sets

	1 Command pools
	*/

	

	//DEVICE


	vkDestroySwapchainKHR(device, swapchain,NULL);

	for(int i=0;i<swapchainImageViewCount;i++){
		
		vkDestroyImageView(device, swapchainImageViews[i],NULL);
	}


	// printf("destrpu images %d\n",swapchainImageCount);

	// for(int i=0;i<swapchainImageCount;i++){
	// 	vkDestroyImage(device, swapchainImages[i],NULL);
	// }






	vkDestroyRenderPass(device, renderPass, NULL);
	
	for(int i=0;i < MAX_FRAMES_IN_FLIGHT;i++)
	{
		vkDestroySemaphore(device,presentCompleteSemaphore[i],NULL);
	
		vkDestroySemaphore(device,renderFinishedSemaphore[i],NULL);

		vkDestroyFence(device,inFlightFences[i],NULL);
	}
	
	

	vkDestroyCommandPool(device, graphicsCommnadPool,NULL);

	vkDestroyCommandPool(device, transferCommnadPool,NULL);
	
	vkDestroyPipeline(device, graphicsPipeline,NULL);

	vkDestroyPipelineLayout(device,pipelineLayout, NULL);

	vkDestroyShaderModule(device,shaderModuleFrag,NULL);
	
	vkDestroyShaderModule(device,shaderModuleVert,NULL);

	vkDestroyBuffer(device,vertextBuffer,NULL);
	

	vkFreeMemory(device, deviceMemory, NULL);

	vkDestroyDevice(device, NULL);

	// INSTANCE

	vkDestroySurfaceKHR(instance, surface,NULL);

	vkDestroyInstance(instance,NULL);

	
	glfwDestroyWindow(window);

    glfwTerminate();


}
void initWindow(){
	printf("%s %d %d\n",__FUNCTION__, WIDTH, HEIGHT );

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
 	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	
	
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);

 
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

int main(){
	printf("%s\n", __FUNCTION__ );
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();


	return 0;
};