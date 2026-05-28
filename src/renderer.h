#pragma once
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include "resource.h"
#include "descriptor.h"
#include "device2.h"
#include "instance.h"
#include "platform.h"
#include "shader_common.h"
#include "swapchain.h"
#include "pipeline.h"
#include "scene.h"




struct FrameData {
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

	VkSemaphore imageAvailable;
	VkSemaphore renderFinished;

	VkFence inFlightFence;

	uint32_t imageIndex;

	struct Buffer buffer_global;
	struct Buffer buffer_global_camera;
	struct Buffer buffer_global_light;

	struct Buffer buffer_instances;
	struct Image depth_image;

	struct Mesh meshes[MAX_MESHES];
	uint32_t mesh_count;
};
struct Renderer_info{

	struct Device_State * ref_device;
	struct Instance_State * ref_instance;
	struct Platform_State * ref_platform;
	struct DescriptorContext * ref_descriptor;
	struct Scene * ref_scene;
};
struct Renderer {

	struct Device_State * ref_device;
	struct Platform_State * ref_platform;
	struct Scene * ref_scene;
	// struct Instance_State * ref_instance;
	// struct Platform_State * ref_platform;
	// struct DescriptorContext * ref_descriptor;
	
	struct Swapchain swapchain;
	struct GraphicsPipeline pipeline;

	VkDescriptorSet desc_set_globals[MAX_FRAMES_IN_FLIGHT];

	VkDescriptorSet desc_set_instances[MAX_FRAMES_IN_FLIGHT];
	VkDescriptorSet desc_set_materials;
	VkDescriptorSet desc_set_samplers;

	struct FrameData frames[MAX_FRAMES_IN_FLIGHT];
	uint32_t current_frame;

	struct Buffer buffer_vertex;
	uint32_t buffer_vertex_used;
	struct Buffer buffer_index;
	uint32_t buffer_index_used;

	struct Buffer buffer_materials;

	struct Texture textures[MAX_TEXTURES];
	uint32_t texture_cnt;

	bool framebuffer_resized;
};

void Renderer_Init(struct Renderer_info* info, struct Renderer* renderer);

void Renderer_Render(struct Renderer* renderer,

	float time,
	float delta_time);

void Renderer_Destroy(struct Renderer* renderer);

struct Texture* Renderer_NewTexture(
	struct Renderer* renderer, void* data, uint32_t size);

struct Texture* Renderer_GetTexture(
	struct Renderer* renderer, uint32_t position);

void Renderer_AppendToIndexBuffer(
	struct Renderer* renderer, uint32_t* indices, uint32_t indices_cnt);
void Renderer_AppendToVertexBuffer(
	struct Renderer* renderer, struct Vertex* vertices, uint32_t vertex_cnt);
