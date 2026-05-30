#pragma once
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "../resource.h"
#include "../descriptor.h"
#include "../device2.h"
#include "../instance.h"
#include "../platform.h"
#include "../shader_common.h"
#include "../swapchain.h"
#include "../pipeline.h"
#include "../scene.h"


struct Render_Frame{
	struct Buffer buffer_global;
	struct Buffer buffer_global_camera;
	struct Buffer buffer_global_light;
	struct Buffer buffer_instances;
	
	struct Image depth_image;

	// struct Mesh meshes[MAX_MESHES];
	// uint32_t mesh_count;	



	VkDescriptorSet desc_set[16];
	uint32_t desc_set_length;

	
	// VkDescriptorSet desc_set_globals;
	// VkDescriptorSet desc_set_instances;
	// VkDescriptorSet desc_set_materials;
	// VkDescriptorSet desc_set_samplers;

	

	// VkDescriptorSet desc_set_globals[MAX_FRAMES_IN_FLIGHT];

	// VkDescriptorSet desc_set_instances[MAX_FRAMES_IN_FLIGHT];
	// VkDescriptorSet desc_set_materials;
	// VkDescriptorSet desc_set_samplers;
};

struct Render_Target{


	struct Pipeline_State pipeline;


	


	struct Render_Frame frames[MAX_FRAMES_IN_FLIGHT];
	
	uint32_t current_frame;

	struct Buffer buffer_vertex;
	uint32_t buffer_vertex_used;
	
	struct Buffer buffer_index;
	uint32_t buffer_index_used;

	struct Buffer buffer_materials;

	struct Texture textures[MAX_TEXTURES];
	uint32_t texture_cnt;	
};
