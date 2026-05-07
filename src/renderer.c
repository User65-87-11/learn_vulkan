#include <vulkan/vulkan_core.h>
#include "renderer.h"

#include "platform.h"
#include "pipeline.h"
#include "instance.h"
#include "swapchain.h"
#include "vertex.h"
#include "shader.h"
#include "device.h"
#include "util/common.h"
#include "resource.h"
#include "descriptor.h"
#include "shader_common.h"




static struct GraphicsPipeline pipeline;

static struct Swapchain swapchain;

struct FrameData g_frames[MAX_FRAMES_IN_FLIGHT] = {};

// static struct Image depth[MAX_FRAMES_IN_FLIGHT];

static uint32_t currentFrame = 0;


static VkDescriptorSet descriptor_set_instances[MAX_FRAMES_IN_FLIGHT]; 
static VkDescriptorSet descriptor_set_globals[MAX_FRAMES_IN_FLIGHT]; 
static VkDescriptorSet descriptor_set_materials; 
static VkDescriptorSet descriptor_set_samplers; 

static char * path_basic_vert = "shaders/out/vert.spv";
static char * path_basic_frag = "shaders/out/frag.spv";



static struct Global_ubo data_globals;
static struct GmArray array_data_materials;
static struct GmArray array_data_instances;



// static VkQueue graphicsQueue = NULL;

// static VkQueue transferQueue = NULL;

// static uint32_t queueFamilyIndexCount = 2;

// static uint32_t graphicsQueueFamilyArrayIndex = 0;

// static uint32_t transferQueueFamilyArrayIndex = 1;

// static uint32_t queueFamilyIndeces[2] = {
//     -1,
//     -1,
// };


static struct GeometryBuffer g_geometry;


// static void AllocateCommandBuffer(VkCommandPool pool, VkCommandBuffer * out);

static void createSyncObjects();

void Renderer_Init(
){

	VkInstance instance = Instance_getInstance();
	VkSurfaceKHR surface = Platform_GetSurface();
	VkDevice device = Device_Get()->device;
	VkPhysicalDevice physicalDevice = Device_Get()->physical_device;

	
	VkExtent2D extent;
	Platform_GetFramebufferSize(&extent.width, &extent.height);





	for(int i=0;i<MAX_FRAMES_IN_FLIGHT ; i++)
	{
		struct FrameData * data = &g_frames[i];
		Device_AllocateCommandBuffer(Device_Get()->graphics_pool,&data->commandBuffer);
	}

	
	
	Swapchain_Create(
		&swapchain,
		device,
		physicalDevice,
		surface,
		extent
	); 

	struct GraphicsPipelineCreateInfo create_info = {};


	create_info.vertexShader = Shader_CreateFromFile(device,path_basic_vert);
	create_info.fragmentShader = Shader_CreateFromFile(device,path_basic_frag);

 	struct DescriptorContext* ctx = Descriptor_GetContext();
	
	VkDescriptorSetLayout layouts[] = {
		ctx->globalLayout, 
		ctx->instanceLayout,
		ctx->materialLayout, 
		ctx->samplerLayout
	};
	
	create_info.descriptorSetLayouts = layouts;
	create_info.descriptorSetLayoutCount = ARR_LEN(layouts);

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
 		
   		descriptor_set_globals[i]= Descriptor_Allocate( ctx->globalLayout);
     	descriptor_set_instances[i] =Descriptor_Allocate( ctx->instanceLayout);
     }
	descriptor_set_materials = Descriptor_Allocate( ctx->materialLayout);
	descriptor_set_samplers = Descriptor_Allocate( ctx->samplerLayout);
	

	
	create_info.depthFormat = swapchain.depthFormat;
	create_info.colorFormat = swapchain.surfaceFormat;
	
	
	
	Pipeline_CreateGraphics(&create_info,&pipeline);

	/*

		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		VK_BUFFER_USAGE_TRANSFER_DST_BIT

		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		VK_MEMORY_PROPERTY_HOST_CACHED_BIT
	 */

	for(int i=0; i< MAX_FRAMES_IN_FLIGHT ; i++){
		struct FrameData * f = &g_frames[i];
		
		Resource_CreateBuffer(
			sizeof(struct Global_ubo)*MAX_INSTANCES, 
			BUFFER_UBO_USEAGE, 
			BUFFER_UBO_PROPS,
			&f->ubo_global
		);

		Resource_CreateBuffer(
			sizeof(struct Instance_ssbo)*MAX_INSTANCES, 
			BUFFER_SSBO_USEAGE, 
			BUFFER_SSBO_PROPS, 
			&f->ssbo_instances
		);
	}
	

}
void Renderer_Render(float time, float delta_time){

	struct FrameData* frame = &g_frames[currentFrame];

	VkDevice device = Device_Get()->device;
	
	VkResult result =
      vkWaitForFences(device, 1, &frame->inFlightFence, VK_TRUE, UINT64_MAX);

  if (result != VK_SUCCESS) {
    EXIT_CLEAN("failed to wait for fence!");
  }

  uint32_t imageIndex = -1;

  result =
      vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
                            frame->presentCompleteSemaphore, NULL, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {

    recreateSwapChain();

    return;
  }

  updateGameObjects(frameIndex);

  updateBuffers(frameIndex);

  vkResetFences(device, 1, &frame->inFlightFence);

  recordCommandBuffer(imageIndex, frameIndex);

  VkPipelineStageFlags2 stageMask =
      VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo2 submitInfo2 = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,

      .waitSemaphoreInfoCount = 1,
      .pWaitSemaphoreInfos =
          &(VkSemaphoreSubmitInfo){
              .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
              .semaphore = frame->presentCompleteSemaphore,
              .stageMask = stageMask,
              .value = 0,
              .deviceIndex = 0,

          },

      .commandBufferInfoCount = 1,
      .pCommandBufferInfos =
          &(VkCommandBufferSubmitInfo){
              .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
              .commandBuffer = frame->graphicsCommandBuffers,

          },
      .signalSemaphoreInfoCount = 1,
      .pSignalSemaphoreInfos =
          &(VkSemaphoreSubmitInfo){
              .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
              .semaphore = frame->renderFinishedSemaphore,
              .stageMask = stageMask,

          },
  };

  vkQueueSubmit2(graphicsQueue, 1, &submitInfo2, frame->inFlightFence);

  VkPresentInfoKHR presentInfo = {

      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &frame->renderFinishedSemaphore,
      .swapchainCount = 1,
      .pSwapchains = &swapchain,
      .pImageIndices = &imageIndex,

  };

  result = vkQueuePresentKHR(graphicsQueue, &presentInfo);

  if ((result == VK_SUBOPTIMAL_KHR) || (result == VK_ERROR_OUT_OF_DATE_KHR) ||
      framebufferResized) {
    framebufferResized = false;
    recreateSwapChain();
  } else {
    assert(result == VK_SUCCESS);
  }
  frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}








static void createSyncObjects() {

  PRINT_FNAME;
  VkDevice device = Device_Get();

		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			
			struct FrameData *frame = &g_frames[i];
			
			VkSemaphoreCreateInfo semaphoreCreateInfo = {
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			};
			
			vkCreateSemaphore(device, 
				&semaphoreCreateInfo, 
				NULL,
				&frame->imageAvailable
			);
			
			vkCreateSemaphore(
				device, 
				&semaphoreCreateInfo, 
				NULL,
				&frame->renderFinished
			);
			
			VkFenceCreateInfo createInfo = {
				.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.flags = VK_FENCE_CREATE_SIGNALED_BIT,
			};
			
			vkCreateFence(device, &createInfo, NULL, &frame->inFlightFence);
		}

}	