#pragma once

#include <vulkan/vulkan_core.h>

#include "device.h"
#include "input.h"
#include "platform.h"
#include "resource.h"
#include "shader/common.h"
#include "common.h"


struct Pipeline_State {
	// struct Device_State * ref_device;

	VkPipeline handle;
	VkPipelineLayout layout;

	VkShaderModule vertexShader;
	VkShaderModule fragmentShader;
};



struct Frame {
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

	VkSemaphore imageAvailable;
	VkSemaphore renderFinished;

	VkFence inFlightFence;

	uint32_t imageIndex;
} ;

struct Swapchain {
	// struct Device_State * ref_device;
	// struct Platform_State * ref_platform;
	
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

struct Mess {
	struct Device_State* ref_device;
	struct Input_State* ref_input;
	struct Input_Backend* ref_backend;
	struct Platform_State* ref_platform;

	struct Layout {
		VkDescriptorPool pool;

		VkDescriptorSetLayout globalLayout;

		VkDescriptorSetLayout instanceLayout;
		VkDescriptorSetLayout materialLayout;
		VkDescriptorSetLayout textureLayout;
		VkDescriptorSetLayout noiseTextureLayout;
		VkDescriptorSetLayout storageImageLayout;
		VkDescriptorSetLayout samplerLayout;

	} Layout;

	struct Pipeline_State pipeline_main;
	
	
	struct Frame frame[MAX_FRAMES_IN_FLIGHT];

	struct GPU_Objects {
		struct Buffer buffer_global[MAX_FRAMES_IN_FLIGHT];
		struct Buffer buffer_global_camera[MAX_FRAMES_IN_FLIGHT];
		struct Buffer buffer_global_light[MAX_FRAMES_IN_FLIGHT];

		

		
		struct Buffer buffer_instances[MAX_FRAMES_IN_FLIGHT];
		struct Image depth_image[MAX_FRAMES_IN_FLIGHT];

		struct Buffer buffer_vertex;
		uint32_t buffer_vertex_used;

		struct Buffer buffer_index;
		uint32_t buffer_index_used;

		struct Buffer buffer_materials;

		struct Sampler sampler;

		struct Image textures [MAX_TEXTURES];
		uint32_t texture_cnt;

		struct Image img_texture0;
		struct Image img_texture_noise0;
		struct Image img_buffer_image0;
		
	} gpu_objects;

	struct Descriptor_Sets{
		VkDescriptorSet set_global[MAX_FRAMES_IN_FLIGHT];
		VkDescriptorSet set_instance[MAX_FRAMES_IN_FLIGHT];
		VkDescriptorSet set_material;
		VkDescriptorSet set_textures;
		VkDescriptorSet set_texture_noise;
		VkDescriptorSet set_buffer_image;
		VkDescriptorSet set_sampler;
	} sets;

	// struct DescriptorContext descriptor;
	struct Swapchain swapchain;

	struct CPU_Data {

		struct Mesh meshes[MAX_MESHES];

		uint32_t mesh_count;

		// struct CameraData camera;

		struct MaterialData material_data[MAX_MATERIALS];
		uint32_t material_count;

		struct InstanceData instance_data[MAX_INSTANCES];
		uint32_t instance_count;

		struct GlobalData global_data;
		struct CameraData camera_data;
		struct LightData light_data;

		struct Scene_Root root;

		struct Scene_Node * nodes;
		uint32_t node_count;
		
	} cpu_data;

	// struct Platform_callback callbacks[2];

	bool buffer_resized;

	uint32_t width, height;

	uint32_t current_frame;

	double time_start;
	double time_last;
	double time_delta;
};

void Mess_Init(
	struct Device_State* device,
	struct Input_State* input,
	struct Input_Backend* input_back,
	struct Platform_State* platform,
	
	struct Mess* ref);


void Mess_Proc(struct Mess* ref);
void Mess_Clean(struct Mess* ref);
