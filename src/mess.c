#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

#define CGLM_FORCE_DEPTH_ZERO_TO_ONE

#include "mess.h"
#include "cglm/mat4.h"
#include "config.h"
#include "input.h"
#include "loader/loader.h"
#include "platform.h"
#include "descriptor.h"
#include "shader.h"
#include "shader/common.h"
#include "shader/binding_main.h"
#include "common.h"
#include "pipelines/pipeline.h"
#include "resource2.h"



static const float yaw = -230.0f;
static const float pitch = 30.0f;



static void Mess_Draw(struct Mess* ref);
static void Mess_Update(struct Mess* ref);

static void Descriptors_Allocate(struct Mess * ref);


static void Free_CPU_Data(struct Mess* ref);

static void Scene_callback_FrameBuffer_Resize(void * mess,uint32_t w,uint32_t h);

static void Swapchain_createImageViews(struct Mess* ref);

static void Swapchain_createImageView(
	struct Mess * ref,
	VkImage image,
	VkFormat format,
	VkImageAspectFlagBits aspectFlags,
	VkImageView* view);

static void  Renderer_callback_FrameBuffer_Resize(void * mess,uint32_t w,uint32_t h);

static void Swapchain_Destroy(struct Mess* ref);

static void update_camera(
	struct Mess* ref,  float dt
);


static void update_keys(
	struct Mess* ref,
	float dt
);

static void Swapchain_Create(
	struct Mess * ref

);
static void renderMainPass(
	struct Mess * ref,
	uint32_t frame_index,
	uint32_t imageIndex
);

static void render_grid(
	struct Mess * ref,
	uint32_t frame_index,
	uint32_t imageIndex );

static void recordCommandBuffer(
	struct Mess * mess,
	uint32_t imageIndex,
	uint32_t frameIndex
);

static void writeBuffers(
	struct Mess* ref
	
);
static void Swapchain_Recreate(struct Mess * ref);


static void Buffers_Init(struct Mess * ref);

static void camera_perspective_init(
	struct CameraData* cam,
	float fov,
	float near_plane,
	float far_plane,
	float aspect_ratio,
	float yaw,
	float pitch,
	vec3 pos
);
// static void createDeapthImage(struct Mess * ref,
// 	uint32_t width,
// 	uint32_t height,
// 	uint32_t mip_levels,
// 	VkFormat format,
// 	VkImageTiling tiling,
// 	VkImageUsageFlags usage,
// 	VkMemoryPropertyFlags properties
// );

// static void Pipeline_CreateGraphics( struct Mess * ref) ;
static void createSyncObjects(struct Mess* ref) ;


static void Descriptor_Init(struct Mess *mess);

static void Free_CPU_Data(struct Mess* ref){

	if(ref->cpu_data.mesh_cap > 0)
	{
		gm_free_aligned(ref->cpu_data.meshes);
		ref->cpu_data.meshes = NULL;
		ref->cpu_data.mesh_cap = 0; 
	}

	if(ref->cpu_data.material_cap > 0)
	{
		gm_free_aligned(ref->cpu_data.material_data);
		ref->cpu_data.material_data = NULL;
		ref->cpu_data.material_cap = 0; 
	}

	if(ref->cpu_data.instance_cap > 0)
	{
		gm_free_aligned(ref->cpu_data.instance_data);
		ref->cpu_data.instance_data = NULL;
		ref->cpu_data.instance_cap = 0; 
	}
	for(int i=0;i<ref->cpu_data.node_count;i++)
	{
		struct Scene_Node * node = &ref->cpu_data.nodes[i];
		if(node->dynamic_nodes && node->child_nodes_ids){
			free(node->child_nodes_ids);
			node->child_cnt = 0;
			node->child_nodes_ids = NULL;
		}
	}
	if(ref->cpu_data.nodes){
		free(ref->cpu_data.nodes);
		ref->cpu_data.nodes = NULL;
		ref->cpu_data.node_count = 0;
	}
}

void Mess_Clean(
	struct Mess * ref
){

	Free_CPU_Data(ref);

	//
	
	Swapchain_Destroy(ref);


	
	VkDevice device = ref->ref_device->logical_device;

	Pipeline_destory(ref->ref_device,&ref->pipeline_main);
	

	Pipeline_destory(ref->ref_device,&ref->pipeline_grid);


	
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		struct Frame * frame = &ref->frame[i];
		
		
		vkDestroyFence(device, frame->inFlightFence, NULL);

		vkDestroySemaphore(device, frame->imageAvailable, NULL);
		vkDestroySemaphore(device, frame->renderFinished, NULL);

		Resource_FreeBuffer(ref->ref_device,&ref->gpu_objects.buffer_global[i]);
		Resource_FreeBuffer(ref->ref_device,&ref->gpu_objects.buffer_global_light[i]);
		Resource_FreeBuffer(ref->ref_device,&ref->gpu_objects.buffer_global_camera[i]);
		Resource_FreeBuffer(ref->ref_device,&ref->gpu_objects.buffer_instances[i]);
		
		Resource_FreeImage(ref->ref_device,&ref->gpu_objects.depth_image[i]);
	}
	Resource2_FreeTexture(ref->ref_device,&ref->gpu_objects.img_texture0);
	Resource2_FreeTexture(ref->ref_device,&ref->gpu_objects.img_texture_noise0);
	Resource2_FreeTexture(ref->ref_device,&ref->gpu_objects.img_buffer_image0);
	
	for (int i = 0; i < ref->gpu_objects.texture_cnt; i++) {
		struct Image texture = ref->gpu_objects.textures[i];
		Resource2_FreeTexture(ref->ref_device,&texture);
	}
	ref->gpu_objects.texture_cnt =0;

	Resource_FreeSampler(ref->ref_device, &ref->gpu_objects.sampler);

	Resource_FreeBuffer(ref->ref_device,&ref->gpu_objects.buffer_index);
	Resource_FreeBuffer(ref->ref_device,&ref->gpu_objects.buffer_vertex);
	Resource_FreeBuffer(ref->ref_device,&ref->gpu_objects.buffer_materials);



		

	vkDestroyDescriptorPool(device, ref->Layout.pool, NULL);

	vkDestroyDescriptorSetLayout(device, ref->Layout.globalLayout, NULL);
	vkDestroyDescriptorSetLayout(device, ref->Layout.instanceLayout, NULL);
	vkDestroyDescriptorSetLayout(device, ref->Layout.materialLayout, NULL);
	vkDestroyDescriptorSetLayout(device, ref->Layout.textureLayout, NULL);
	vkDestroyDescriptorSetLayout(device, ref->Layout.noiseTextureLayout, NULL);
	vkDestroyDescriptorSetLayout(device, ref->Layout.storageImageLayout, NULL);
	vkDestroyDescriptorSetLayout(device, ref->Layout.samplerLayout, NULL);
	
	// Swapchain_Destroy(&app_info.renderer.swapchain);

	// Renderer_Destroy(&app_info.renderer);

	// Descriptor_Destroy(&app_info.descriptor);
}
void Mess_Init(
	struct Device_State* device,
	struct Input_State* input,
	struct Input_Backend* input_back,
	struct Platform_State* platform,

	struct Mess * ref
	
){

	memset(ref, 0, sizeof(struct Mess));
	ref->ref_platform = platform;
	ref->ref_input = input;
	ref->ref_device = device;
	ref->ref_backend= input_back;

	ref->cpu_data.mesh_cap = MAX_MESHES;
	ref->cpu_data.mesh_count = 0;
	ref->cpu_data.meshes = gm_alloc_aligned(ref->cpu_data.mesh_cap* sizeof(struct Mesh),32);
	
	 ref->cpu_data.instance_cap = MAX_INSTANCES;
	ref->cpu_data.instance_count = 0;
	ref->cpu_data.instance_data = gm_alloc_aligned(ref->cpu_data.instance_cap * sizeof(struct InstanceData),32);

	
	 ref->cpu_data.material_cap = MAX_MATERIALS;
	 ref->cpu_data.material_count = 0;
	  ref->cpu_data.material_data = gm_alloc_aligned(ref->cpu_data.material_cap * sizeof(struct MaterialData),32);
		

	
	struct Platform_callback callbacks[2];
	callbacks[0].callback_resize = Scene_callback_FrameBuffer_Resize;
	callbacks[0].dst = ref;

	callbacks[1].callback_resize = Renderer_callback_FrameBuffer_Resize;
	callbacks[1].dst = ref;

	Platform_setFrameResizeCallBack(ref->ref_platform ,callbacks ,ARR_LEN(callbacks));

	
	Descriptor_Init(ref);



 	Platform_GetFramebufferSize(ref->ref_platform,&ref->width,&ref->height);

	{
		vec3 pos0 = {2.0, -2.0f, -2.0f};

		camera_perspective_init(
			&ref->cpu_data.camera_data, 45.0, 0.1f, 100.0f,
			(float)ref->width / ref->height, yaw, pitch, pos0
		);

		ref->cpu_data.global_data.frame_cnt = 0x88;
		ref->cpu_data.global_data.framebuffer_size[0] = ref->width;
		ref->cpu_data.global_data.framebuffer_size[1] = ref->height;
	}


	
	createSyncObjects(ref);


	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		struct Frame* data = &ref->frame[i];
		Device_AllocateCommandBuffer(
			ref->ref_device,
			ref->ref_device->command_pool_graphics, 
			&data->commandBuffer
		);
	}

	Swapchain_Create(
		ref
	);


	

	Buffers_Init(ref);

	Descriptors_Allocate(ref);

	Pipeline_Create_Main(ref);
	
	Pipeline_Create_Grid(ref) ;
}



static void  Renderer_callback_FrameBuffer_Resize(void * mess,uint32_t w,uint32_t h){
	PRINT_FNAME;

	struct Mess * ref = mess;
	ref->buffer_resized = true;
}


static void update_camera_perspective(struct CameraData * cam, float aspect){

	cam->aspect_ratio = aspect;


	
	
	glm_perspective_rh_zo(
		glm_rad(cam->fov),
		cam->aspect_ratio, 
		cam->near,
		cam->far, 
		cam->proj
	);
	// cam->proj[1][1] *= -1.f;

	glm_mat4_mul(
		cam->proj, 
		cam->view,
		cam->view_proj
	);

	glm_mat4_dup(cam->view, cam->inv_view);
	glm_mat4_inv(cam->inv_view, cam->inv_view);
	
	glm_mat4_dup(cam->proj, cam->inv_proj);
	glm_mat4_inv(cam->inv_proj, cam->inv_proj);
}



static void Scene_callback_FrameBuffer_Resize(void * mess,uint32_t w,uint32_t h){
	PRINT_FNAME;


	struct Mess * s = mess;

	
	s->cpu_data.global_data.framebuffer_size[0] = w;	
	s->cpu_data.global_data.framebuffer_size[1] = h;

	
	update_camera_perspective(&s->cpu_data.camera_data,(float)w/h);
}


static void Descriptor_Init(struct Mess *mess) {
	PRINT_FNAME;
	
	
	// context->device_ref = info->device_ref;

	uint32_t cnt_glob = MAX_FRAMES_IN_FLIGHT * 3;
	uint32_t cnt_inst = MAX_FRAMES_IN_FLIGHT;
	uint32_t cnt_material = 1;
	uint32_t cnt_textures = MAX_TEXTURES ;
	uint32_t cnt_noise_texture = 1;
	uint32_t cnt_sampler = 1;
	uint32_t cnt_buffer_image = 1;
	uint32_t max_sets = cnt_inst + cnt_glob + cnt_material + cnt_textures +cnt_sampler + cnt_noise_texture + cnt_buffer_image;

	VkDevice device = mess->ref_device->logical_device;
	

	VkDescriptorPoolSize poolSizes[] = {

		(VkDescriptorPoolSize){
			.type = VK_DESCRIPTOR_TYPE_SAMPLER,
			.descriptorCount = 1
		},
		(VkDescriptorPoolSize){
			.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
			.descriptorCount = MAX_TEXTURES 
			+  1 // white noice texture
		},

		(VkDescriptorPoolSize){
			.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
			.descriptorCount = 1 
		},
		
		
		(VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = MAX_FRAMES_IN_FLIGHT * 3},
		
		(VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount =
				MAX_FRAMES_IN_FLIGHT * MAX_INSTANCES + MAX_MATERIALS},

	};

	VkDescriptorPoolCreateInfo poolInfo = {

		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.pNext = NULL,
		// .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		.maxSets = max_sets,
		.poolSizeCount = ARR_LEN(poolSizes),
		.pPoolSizes = poolSizes};

	VkResult result =
		vkCreateDescriptorPool(device, &poolInfo, NULL,  &mess->Layout.pool);


	// BINDING!!!!!!!!!!!!!
	{


		VkDescriptorSetLayoutBinding bindings[3] = {
			{.binding = MAIN_BINDING_GLOBAL_GLOBAL,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags =
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},

			{.binding = MAIN_BINDING_GLOBAL_CAMERA,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags =
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},

			{.binding = MAIN_BINDING_GLOBAL_LIGHT,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags =
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT}
		};

		VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = ARR_LEN(bindings),
			.pBindings = bindings};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &globalLayoutInfo, NULL, &mess->Layout.globalLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN("vkCreateDescriptorSetLayout4 failed");
		}
	}

	{
		VkDescriptorSetLayoutBinding materialBinding = {
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount = 1,
			.stageFlags =
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
		};

		VkDescriptorSetLayoutCreateInfo materialLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &materialBinding};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &materialLayoutInfo, NULL, &mess->Layout.materialLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN(
				"vkCreateDescriptorSetLayout  materialDescriptorSetLayout");
		}
	}

	{
		VkDescriptorSetLayoutBinding instanceBindings = {

			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount = 1,
			.stageFlags =
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutCreateInfo instanceLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &instanceBindings};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &instanceLayoutInfo, NULL, &mess->Layout.instanceLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN(
				"vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
		}
	}

	{
		VkDescriptorSetLayoutBinding samplerBindings = {

			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT

		};

		VkDescriptorSetLayoutCreateInfo samplerLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &samplerBindings
		};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &samplerLayoutInfo, NULL, &mess->Layout.samplerLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN("vkCreateDescriptorSetLayout samplerBindings");
		}
	}

	{
		VkDescriptorSetLayoutBinding textureBindings = {

			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
			.descriptorCount = MAX_TEXTURES,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT

		};

		VkDescriptorSetLayoutCreateInfo textureLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &textureBindings
		};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &textureLayoutInfo, NULL, &mess->Layout.textureLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN("vkCreateDescriptorSetLayout samplerBindings");
		}
	}

	{
		VkDescriptorSetLayoutBinding noiseTexBindings = {

			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT

		};

		VkDescriptorSetLayoutCreateInfo noiseTexLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &noiseTexBindings
		};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &noiseTexLayoutInfo, NULL, &mess->Layout.noiseTextureLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN("vkCreateDescriptorSetLayout samplerBindings");
		}
	}
	{
		VkDescriptorSetLayoutBinding storageImageBinding  = {

			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT

		};

		VkDescriptorSetLayoutCreateInfo storageImageLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &storageImageBinding
		};

		VkResult res = vkCreateDescriptorSetLayout(
			device, &storageImageLayoutInfo, NULL, &mess->Layout.storageImageLayout);

		if (res != VK_SUCCESS) {
			EXIT_CLEAN("vkCreateDescriptorSetLayout samplerBindings");
		}
	}
}

static void camera_perspective_init(
	struct CameraData* cam,
	float fov,
	float near_plane,
	float far_plane,
	float aspect_ratio,
	float yaw,
	float pitch,
	vec3 pos
){


	
	cam->fov = fov;
	cam->near = near_plane;
	cam->far = far_plane;
	cam->aspect_ratio = aspect_ratio;

	cam->yaw = yaw;
	cam->pitch = pitch;
	cam->ortho_size = 0.0f;



	

	GLM_VEC3_COPY(cam->pos, pos);
	GLM_VEC3_SET(cam->up,0.0,-1.0,0.0);

	printf("CAM: %f,%f,%f\n",cam->pos);


	glm_perspective_rh_zo(
		glm_rad(cam->fov),
		cam->aspect_ratio, 
		cam->near,
		cam->far, 
		cam->proj
	);
	
	// glm_mat4_identity(	cam->view);

	vec3 front_;

	front_[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
	front_[1] = sin(glm_rad(cam->pitch));
	front_[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));

	GLM_VEC3_COPY(cam->front, front_);

	vec3 cameraCenter;
	
	glm_vec3_add(
		cam->pos, 
		cam->front, 
		cameraCenter
	);
	
	glm_lookat_rh(
		cam->pos, 
		cameraCenter, 
		cam->up,
		cam->view
	);

	glm_mat4_mul(
		cam->proj, 
		cam->view,
		cam->view_proj
	);
	
}


static void Descriptors_Allocate(struct Mess * ref){
	
	Descriptor_Allocate(
		ref->ref_device->logical_device,
		ref->Layout.samplerLayout,
		ref->Layout.pool,
		&ref->sets.set_sampler,
		1
	);
	
	Descriptor_UpdateSamplerDescriptors(
		ref->ref_device->logical_device,
		ref->sets.set_sampler, 
		0,
		ref->gpu_objects.sampler.handle
	);
	Descriptor_Allocate(
		ref->ref_device->logical_device,
		ref->Layout.textureLayout, 
		ref->Layout.pool, 
		&ref->sets.set_textures,
		1);

	for(int i=0;i<MAX_TEXTURES;i++){
	
	
		Descriptor_UpdateTextureDescriptors(
			ref->ref_device->logical_device,
			ref->sets.set_textures,
			0,
			ref->gpu_objects.img_texture0.view,
			i);
	}

	Descriptor_Allocate(
		ref->ref_device->logical_device,
		ref->Layout.storageImageLayout, 
		ref->Layout.pool, 
		&ref->sets.set_buffer_image,
		1);


	Descriptor_UpdateBufferImageDescriptors(
		ref->ref_device->logical_device,
		ref->sets.set_buffer_image,
		0,
		ref->gpu_objects.img_buffer_image0.view,
		0);

	Descriptor_Allocate(
		ref->ref_device->logical_device,
		ref->Layout.textureLayout, 
		ref->Layout.pool, 
		&ref->sets.set_texture_noise,
		1);

	Descriptor_UpdateTextureDescriptors(
		ref->ref_device->logical_device,
		ref->sets.set_texture_noise,
		0,
		ref->gpu_objects.img_texture_noise0.view,
		0);


	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		struct Frame* frame = &ref->frame[i];
		Descriptor_Allocate(
			ref->ref_device->logical_device,
			ref->Layout.globalLayout,
			ref->Layout.pool,
			&ref->sets.set_global[i],
			1
		);
		Descriptor_UpdateBuffer(
			ref->ref_device->logical_device,
			ref->sets.set_global[i],
			MAIN_BINDING_GLOBAL_GLOBAL, 
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			ref->gpu_objects.buffer_global[i].handle,
			ref->gpu_objects.buffer_global[i].size,
			0
		);
		
		Descriptor_UpdateBuffer(
			ref->ref_device->logical_device,
			ref->sets.set_global[i],
			MAIN_BINDING_GLOBAL_CAMERA, 
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			ref->gpu_objects.buffer_global_camera[i].handle,
			ref->gpu_objects.buffer_global_camera[i].size,
			0
		);
		Descriptor_UpdateBuffer(
			ref->ref_device->logical_device,
			ref->sets.set_global[i],
			MAIN_BINDING_GLOBAL_LIGHT, 
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			ref->gpu_objects.buffer_global_light[i].handle,
			ref->gpu_objects.buffer_global_light[i].size,
			0
		);

		Descriptor_Allocate(
			ref->ref_device->logical_device,
			ref->Layout.instanceLayout,
			ref->Layout.pool,
			&ref->sets.set_instance[i],
			1
		);

		Descriptor_UpdateBuffer(
			ref->ref_device->logical_device,
			ref->sets.set_instance[i], 
			0,
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 
			ref->gpu_objects.buffer_instances[i].handle,
			ref->gpu_objects.buffer_instances[i].size,
			0
		
		);
	}

	Descriptor_Allocate(
		ref->ref_device->logical_device,
		ref->Layout.materialLayout,
		ref->Layout.pool,
		&ref->sets.set_material,
		1
	);
	
	Descriptor_UpdateBuffer(
		ref->ref_device->logical_device,
		ref->sets.set_material,
		0,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 
		ref->gpu_objects.buffer_materials.handle,
		ref->gpu_objects.buffer_materials.size,
		0
	);
}

static void Buffers_Init(struct Mess * ref){

	
	Resouce_createSampler(ref->ref_device, &ref->gpu_objects.sampler.handle);


	//dummy texture
	{	
		
		uint32_t tex_data = 0xffffffff;
		uint32_t tex_size = sizeof(tex_data);
	
	
	
		Resource2_CreateTexture(
			ref->ref_device,
			&tex_data,
			1,
			1,
			VK_FORMAT_R8G8B8A8_SRGB,
			&ref->gpu_objects.img_texture0
		);
		printf("here2 \n");
	}
	// buffer_image
	{

		uint32_t img_width = ref->width ;
		uint32_t img_height = ref->height ;
		uint32_t image_size = img_width * img_height;
		uint32_t data_size = image_size * sizeof(float) * 4;
		float  * data = malloc(data_size);
	
		for(int i=0; i < image_size ; i++){
			data[i*4] = (float)rand() / (float)RAND_MAX;
			data[i*4 + 1] = 1.0;
			data[i*4 + 2] = (float)rand() / (float)RAND_MAX;
		}
	
		Resource2_CreateImageBuffer(
			ref->ref_device,
			data,
			data_size,
			img_width,
			img_height,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			&ref->gpu_objects.img_buffer_image0
		);
	
		free(data);

		
	}
	//noise texture
	{	

		struct ImageData temp_data;
		Loader_LoadImageDataFromFile(DEFAULT_NOISE_TEXTURE, &temp_data);
		// uint32_t tex_data = 0xffffffff;
		// uint32_t tex_size = sizeof(tex_data);
	
	
		Resource2_CreateTexture(
			ref->ref_device,
			temp_data.data,
			temp_data.width,
			temp_data.height,
			VK_FORMAT_R8G8B8A8_SRGB,
			&ref->gpu_objects.img_texture_noise0
		);
	

		Loader_FreeImageData(&temp_data);
	}
	

	
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		struct Frame* frame = &ref->frame[i];

		Resource_CreateBuffer(
			ref->ref_device,
			sizeof(struct GlobalData),
		 	BUFFER_UBO_USAGE,
			BUFFER_UBO_PROPS, 
			&ref->gpu_objects.buffer_global[i]
		);
		Resource_mapBufferMemory(
			ref->ref_device,&ref->gpu_objects.buffer_global[i]
		);


		Resource_CreateBuffer(
			ref->ref_device,
			sizeof(struct CameraData), 
			BUFFER_UBO_USAGE,
			BUFFER_UBO_PROPS, 
			&ref->gpu_objects.buffer_global_camera[i]
		);


		Resource_mapBufferMemory(
			ref->ref_device,
			&ref->gpu_objects.buffer_global_camera[i]
		);
	


		Resource_CreateBuffer(
			ref->ref_device,
			sizeof(struct LightData), 
			BUFFER_UBO_USAGE,
			BUFFER_UBO_PROPS, 
			&ref->gpu_objects.buffer_global_light[i]
		);
		Resource_mapBufferMemory(
			ref->ref_device,
			&ref->gpu_objects.buffer_global_light[i]
		);

	

		Resource_CreateBuffer(
			ref->ref_device,
			sizeof(struct InstanceData) * MAX_INSTANCES,
			BUFFER_SSBO_USAGE, BUFFER_SSBO_PROPS, 
			&ref->gpu_objects.buffer_instances[i]
		);

		
		Resource_mapBufferMemory(
			ref->ref_device,
			&ref->gpu_objects.buffer_instances[i]
		);
	

	}

	for(int i=0;i<MAX_FRAMES_IN_FLIGHT;i++)
	{
		
		Resource2_createDeapthImage(ref->ref_device,
			ref->width, ref->height,
			1, // mip levels
			ref->swapchain.depthFormat, 
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			&ref->gpu_objects.depth_image[i]
		);
		
	}
	Resource_CreateBuffer(
		ref->ref_device,
		sizeof(struct MaterialData) * MAX_MATERIALS,
		BUFFER_SSBO_USAGE, 
		BUFFER_SSBO_PROPS, 
		&ref->gpu_objects.buffer_materials
	);
	

	Resource_mapBufferMemory(
		ref->ref_device,
		&ref->gpu_objects.buffer_materials
	);



	Resource_CreateBuffer(
		ref->ref_device,
		MAX_VERTICES * sizeof(struct Vertex),
		BUFFER_VERTEX_USAGE, 
		BUFFER_VERTEX_PROPS, 
		&ref->gpu_objects.buffer_vertex
	);
	ref->gpu_objects.buffer_vertex_used = 0;

	Resource_CreateBuffer(
		ref->ref_device,
		MAX_INDICES * sizeof(uint32_t), 
		BUFFER_INDEX_USAGE,
		BUFFER_INDEX_PROPS, 
		&ref->gpu_objects.buffer_index
	);
	ref->gpu_objects.buffer_index_used = 0;
}



static void createSyncObjects(struct Mess* ref) {

	PRINT_FNAME;
	struct Device_State* device = ref->ref_device;

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		struct Frame* frame = &ref->frame[i];

		VkSemaphoreCreateInfo semaphoreCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};

		vkCreateSemaphore(device->logical_device, &semaphoreCreateInfo, NULL,
			&frame->imageAvailable);

		vkCreateSemaphore(device->logical_device, &semaphoreCreateInfo, NULL,
			&frame->renderFinished);

		VkFenceCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT,
		};

		vkCreateFence(
			device->logical_device, &createInfo, NULL, &frame->inFlightFence);
	}
}




static void Swapchain_Create(
	struct Mess * ref

)
{
	PRINT_FNAME;




	
	// sc->ref_device = info->ref_device;
	// sc->ref_platform = info->ref_platform;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	// VkPhysicalDevice physicalDevice = Device_GetPhysical();
	// VkDevice  device = Device_Get();

	// surface0 = surface;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		ref->ref_device->physical_device, ref->ref_platform->surface, &surfaceCapabilities);

	printf("Surface extent: w:%d h:%d\n",
		surfaceCapabilities.currentExtent.width,
		surfaceCapabilities.currentExtent.height);

	uint32_t physicalDeviceSurfaceFormatCount = 0;

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		ref->ref_device->physical_device, ref->ref_platform->surface, &physicalDeviceSurfaceFormatCount, NULL);

	VkSurfaceFormatKHR surfaceFormats[physicalDeviceSurfaceFormatCount];

	vkGetPhysicalDeviceSurfaceFormatsKHR(ref->ref_device->physical_device, ref->ref_platform->surface,
		&physicalDeviceSurfaceFormatCount, surfaceFormats);

	ref->swapchain.depthFormat = Device_findDepthFormat(ref->ref_device);
	ref->swapchain.surfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;
	printf("Surface formats:\n");

	// swapchainSurfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;

	// swapchainSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	ref->swapchain.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	for (int i = 0; i < physicalDeviceSurfaceFormatCount; i++) {

		if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) {

			printf("\tSurfaceFormat.format: %d\n", surfaceFormats[i].format);
			ref->swapchain.surfaceFormat = surfaceFormats[i].format;
			break;
		}
	}

	for (int i = 0; i < physicalDeviceSurfaceFormatCount; i++) {

		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			printf("\tSurfaceFormat.colorSpace:%d\n", surfaceFormats[i].colorSpace);
			ref->swapchain.colorSpace = surfaceFormats[i].colorSpace;
			break;
		}
	}

	uint32_t presentModeCount = 0;

	vkGetPhysicalDeviceSurfacePresentModesKHR(
	ref->ref_device->physical_device, ref->ref_platform->surface, &presentModeCount, NULL);

	VkPresentModeKHR presentModes[presentModeCount];

	vkGetPhysicalDeviceSurfacePresentModesKHR(
	ref->ref_device->physical_device, ref->ref_platform->surface, &presentModeCount, presentModes);

	printf("Surface pPresentModes:\n");

	uint32_t presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

	for (int i = 0; i < presentModeCount; i++) {

		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			printf("\tpresent mode: %d\n", presentModes[i]);
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}

	ref->swapchain.extent = surfaceCapabilities.currentExtent;

	if (ref->swapchain.extent.width > surfaceCapabilities.maxImageExtent.width &&
		ref->swapchain.extent.height > surfaceCapabilities.maxImageExtent.height) {

		ref->swapchain.extent.width = WIDTH;
		ref->swapchain.extent.height = HEIGHT;
	}

	// uint32_t width, height;
	// Platform_GetFramebufferSize(info->ref_platform,&width, &height);

	if (ref->width >= surfaceCapabilities.minImageExtent.width &&
		ref->width <= surfaceCapabilities.maxImageExtent.width &&
		ref->width >= surfaceCapabilities.minImageExtent.height &&
		ref->width <= surfaceCapabilities.maxImageExtent.height) {
			ref->swapchain.extent = (VkExtent2D){
			.width = ref->width,
			.height = ref->width,
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
ref->swapchain.imageCount = imageCount;

	VkSwapchainCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.minImageCount = ref->swapchain.imageCount,
		.imageFormat = ref->swapchain.surfaceFormat,
		.imageColorSpace = ref->swapchain.colorSpace,
		.imageExtent = ref->swapchain.extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = surfaceCapabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = presentMode,
		.surface = ref->ref_platform->surface,
		.clipped = true,
	};

	vkCreateSwapchainKHR(ref->ref_device->logical_device, &createInfo, NULL, &ref->swapchain.handle);

	Swapchain_createImageViews(ref);
}




static void Swapchain_createImageViews(struct Mess* ref) {

	PRINT_FNAME;

	/*
	INFO

	how many images you want (minImageCount)
	how they should be formatted (imageFormat)
	how they will be used (imageUsage)
	their size (imageExtent)

	*/

	// VkDevice device = Device_Get();

	vkGetSwapchainImagesKHR(ref->ref_device->logical_device, ref->swapchain.handle, &ref->swapchain.imageCount, NULL);

	// VkImage sc_images[image_view_cnt];

	vkGetSwapchainImagesKHR(ref->ref_device->logical_device, ref->swapchain.handle, 
		&ref->swapchain.imageCount, ref->swapchain.images
	);

	printf("pSwapchainImageCount %d\n", &ref->swapchain.imageCount);

	// swapchainImageViewCount = swapchainImageCount;

	for (int i = 0; i < ref->swapchain.imageCount; i++) {

		// swapchain_images[i].handle = sc_images[i];
		// swapchain_images[i].mipLevels = 1;
		Swapchain_createImageView(
			ref,
			ref->swapchain.images[i], 
			ref->swapchain.surfaceFormat,
			VK_IMAGE_ASPECT_COLOR_BIT, 
			&ref->swapchain.image_views[i]
		);
	}
}



static void Swapchain_createImageView(
	struct Mess * ref,
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

	vkCreateImageView(ref->ref_device->logical_device, &viewInfo, NULL, view);
}



static void Swapchain_Destroy(struct Mess* ref) {
	PRINT_FNAME;
	// struct Device* device = Device_Get();

	vkDeviceWaitIdle(ref->ref_device->logical_device);

	for (int i = 0; i < ref->swapchain.imageCount; i++) {
		// cleanImageRes(&swapchain_images[i]);
		vkDestroyImageView(ref->ref_device->logical_device, ref->swapchain.image_views[i], NULL);
	}
	// swapchainImageViewCount = 0;
	ref->swapchain.imageCount = 0;

	vkDestroySwapchainKHR(ref->ref_device->logical_device, ref->swapchain.handle, NULL);

	ref->swapchain.handle = NULL;
}


void Mess_Draw(struct Mess* ref){

	

	
	
	uint32_t frame_index = ref->current_frame;
	
	struct Frame * frame = &ref->frame[frame_index];

	// struct Device* device = Device_Get();

	VK_CHECK(vkWaitForFences(
		ref->ref_device->logical_device, 1, &frame->inFlightFence, VK_TRUE, UINT64_MAX));

	vkResetFences(ref->ref_device->logical_device, 1, &frame->inFlightFence);

	writeBuffers(ref);
	
	uint32_t imageIndex = -1;

	VkResult result = vkAcquireNextImageKHR(
		ref->ref_device->logical_device,
		ref->swapchain.handle, 
		UINT64_MAX, 
		frame->imageAvailable, 
		NULL,
		&imageIndex
	);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		
		Swapchain_Recreate(ref);

		return;
	}

	// updateGameObjects(frame_index);



	recordCommandBuffer(ref, imageIndex, frame_index);


	VkPipelineStageFlags2 stageMask =
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo2 submitInfo2 = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,

		.waitSemaphoreInfoCount = 1,
		.pWaitSemaphoreInfos =
			&(VkSemaphoreSubmitInfo){
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
				.semaphore = frame->imageAvailable,
				.stageMask = stageMask,
				.value = 0,
				.deviceIndex = 0,

			},

		.commandBufferInfoCount = 1,
		.pCommandBufferInfos =
			&(VkCommandBufferSubmitInfo){
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
				.commandBuffer = frame->commandBuffer,

			},
		.signalSemaphoreInfoCount = 1,
		.pSignalSemaphoreInfos =
			&(VkSemaphoreSubmitInfo){
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
				.semaphore = frame->renderFinished,
				.stageMask = stageMask,

			},
	};


	vkQueueSubmit2(
		ref->ref_device->queue_graphics.queue, 
		1, 
		&submitInfo2, 
		frame->inFlightFence
	);

	VkPresentInfoKHR presentInfo = {

		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &frame->renderFinished,
		.swapchainCount = 1,
		.pSwapchains = &ref->swapchain.handle,
		.pImageIndices = &imageIndex,

	};

	result = vkQueuePresentKHR(ref->ref_device->queue_graphics.queue, &presentInfo);

	if ((result == VK_SUBOPTIMAL_KHR) || (result == VK_ERROR_OUT_OF_DATE_KHR) ||
		ref->buffer_resized) {
		ref->buffer_resized = false;


		uint32_t width = 0, height = 0;

		// Wait until window is not minimized
		do {
			Platform_GetFramebufferSize(ref->ref_platform,&width, &height);
			Platform_WaitForEvents(ref->ref_platform);
		} while (width == 0 || height == 0);

		// Ensure GPU is not using swapchain resources
		vkDeviceWaitIdle(ref->ref_device->logical_device);

		VkExtent2D new_extent={
			.width = width,
			.height = height,
		};


		for(int i=0;i<MAX_FRAMES_IN_FLIGHT;i++)
		{
			

			Resource2_createDeapthImage(
				ref->ref_device,
				new_extent.width, new_extent.height,
				1, // mip levels
				ref->swapchain.depthFormat, 
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				&ref->gpu_objects.depth_image[i]
			);
		}
		
		// createDeapthImage(ref,new_extent.width, new_extent.height,
		// 	1, // mip levels
		// 	ref->swapchain.depthFormat, VK_IMAGE_TILING_OPTIMAL,
		// 	VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		// 	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	
		
		Swapchain_Recreate(ref);

		// Swapchain_Recreate(struct Swapchain *sc, VkDevice device,
		// VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
		// recreateSwapChain();
	} else {
		assert(result == VK_SUCCESS);
	}

	ref->current_frame = (frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
}


static void Swapchain_Recreate(struct Mess * ref){

	Swapchain_Destroy(ref);
	Swapchain_Create(ref);
}


static void writeBuffers(
	struct Mess* ref

) {

	// struct Frame* frame = &ref->frame[ref->current_frame];
	uint32_t frame_idx = ref->current_frame;
	struct GPU_Objects * gpu_o = &ref->gpu_objects;
	
	memcpy(
		gpu_o->buffer_instances[frame_idx].mapped, 
		ref->cpu_data.instance_data,
		sizeof(struct InstanceData) * ref->cpu_data.instance_count
	);

	memcpy(
		gpu_o->buffer_global[frame_idx].mapped, 
		&ref->cpu_data.global_data,
		sizeof(struct GlobalData)
	);

	memcpy(
		gpu_o->buffer_global_camera[frame_idx].mapped,
		&ref->cpu_data.camera_data,
		sizeof(struct CameraData)
	);

	memcpy(
		gpu_o->buffer_global_light[frame_idx].mapped,
		&ref->cpu_data.light_data,
		sizeof(struct LightData)
	);

	memcpy(
		gpu_o->buffer_materials.mapped, 
		ref->cpu_data.material_data,
		sizeof(struct MaterialData) * ref->cpu_data.material_count
	);
	
}




static void recordCommandBuffer(
	struct Mess * ref,
	uint32_t imageIndex,
	uint32_t frameIndex) {

	
	struct Frame* frame = &ref->frame[frameIndex];

	VkCommandBuffer commandBuffer = frame->commandBuffer;

	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0,
		.pInheritanceInfo = NULL,
	};
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	Resource_transitionImageLayout(ref->ref_device,
		commandBuffer,
		&ref->swapchain.images[imageIndex], 
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0,
		VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_IMAGE_ASPECT_COLOR_BIT, 1);


	
	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	VkClearValue clearDepth = {{{1.0f, 0}}};

	// struct Frame* frame = &ref->frame[frame_index];
	struct GPU_Objects* gpu_o = &ref->gpu_objects;

	VkRenderingAttachmentInfo colorAttachmentsInfos[] = {

		colorAttachmentsInfos[0] =
			(VkRenderingAttachmentInfo){

				.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
				.imageView = ref->swapchain.image_views[imageIndex],
				.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				// .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.clearValue = clearColor

			}

	};

	VkRenderingAttachmentInfo depthAttachmentInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = gpu_o->depth_image[frameIndex].view,
		.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		// .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.clearValue = clearDepth};

	VkRenderingInfo renderingInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea = 
		{
			.offset = {0, 0}, 
			.extent = ref->swapchain.extent
		},
		.layerCount = 1,
		.colorAttachmentCount = ARR_LEN(colorAttachmentsInfos),
		.pColorAttachments = colorAttachmentsInfos,

		.pDepthAttachment = &depthAttachmentInfo,
	};

	vkCmdBeginRendering(frame->commandBuffer, &renderingInfo);
	
	render_grid(ref, frameIndex, imageIndex);

	// Resource_transitionImageLayout(ref->ref_device,
	// 		commandBuffer,
	// 		&ref->swapchain.images[imageIndex], 
	// 		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	// 		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0,
	// 		VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
	// 		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
	// 		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
	// 		VK_IMAGE_ASPECT_COLOR_BIT, 1);

	
		
	renderMainPass(ref, frameIndex, imageIndex);
	
	vkCmdEndRendering(frame->commandBuffer);

	Resource_transitionImageLayout(ref->ref_device,
		commandBuffer,
		&ref->swapchain.images[imageIndex],
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		0, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		// VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_NONE, VK_IMAGE_ASPECT_COLOR_BIT, 1);

	
	vkEndCommandBuffer(commandBuffer);


}

static void renderMainPass(
	struct Mess * ref,
	uint32_t frame_index,
	uint32_t imageIndex
) {

		struct Frame* frame = &ref->frame[frame_index];
		


	VkViewport viewPort = {.x = 0,
		.y = 0,
		.width = ref->swapchain.extent.width,
		.height = ref->swapchain.extent.height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f};

	vkCmdSetViewport(frame->commandBuffer, 0, 1, &viewPort);

	VkRect2D scissor = {.extent = ref->swapchain.extent, .offset = {0, 0}};
	vkCmdSetScissor(frame->commandBuffer, 0, 1, &scissor);
	
	vkCmdBindPipeline(
		frame->commandBuffer, 
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		ref->pipeline_main.handle
	);


	VkDeviceSize offset = 0;

	vkCmdBindVertexBuffers(
		frame->commandBuffer, 0, 1, &ref->gpu_objects.buffer_vertex.handle, &offset);
	
	vkCmdBindIndexBuffer(frame->commandBuffer, ref->gpu_objects.buffer_index.handle, 0,
		VK_INDEX_TYPE_UINT32);

	VkDescriptorSet dset[7] = {
		ref->sets.set_global[frame_index],
		ref->sets.set_instance[frame_index],
		ref->sets.set_material,
		ref->sets.set_textures,
		ref->sets.set_sampler,
		ref->sets.set_texture_noise,
		ref->sets.set_buffer_image
	};

	/*
	
		#define DESC_SET_GLOBALS 0
#define DESC_SET_INSTANCES 1
#define DESC_SET_MATERIALS 2
#define DESC_SET_TEXTURES 3
#define DESC_SET_SAMPLER 4
#define DESC_SET_NOISE 5
	 */

	vkCmdBindDescriptorSets(
		frame->commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS, 
		ref->pipeline_main.layout, 0,
		ARR_LEN(dset), dset, 0, NULL
	);



	for (int i = 0; i < ref->cpu_data.mesh_count; i++) {


		struct Mesh* mesh = &ref->cpu_data.meshes[i];

		
		vkCmdDrawIndexed(
			frame->commandBuffer,
			mesh->index_count, 
			mesh->instance_cnt, 
			mesh->index_offset, 
			mesh->vertex_offset,
			mesh->instance_offset
		);
	}
	


}



static void render_grid(
	struct Mess * ref,
	uint32_t frame_index,
	uint32_t imageIndex) {

	// VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	// VkClearValue clearDepth = {{{1.0f, 0}}};

	struct Frame* frame = &ref->frame[frame_index];
	struct GPU_Objects* gpu_o = &ref->gpu_objects;

	// VkRenderingAttachmentInfo colorAttachmentsInfos[] = {

	// 	colorAttachmentsInfos[0] =
	// 		(VkRenderingAttachmentInfo){

	// 			.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
	// 			.imageView = ref->swapchain.image_views[imageIndex],
	// 			.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	// 			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
	// 			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	// 			.clearValue = clearColor

	// 		}

	// };

	// VkRenderingAttachmentInfo depthAttachmentInfo = {
	// 	.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
	// 	.imageView = gpu_o->depth_image[frame_index].view,
	// 	.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	// 	.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
	// 	.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
	// 	.clearValue = clearDepth};

	// VkRenderingInfo renderingInfo = {
	// 	.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
	// 	.renderArea = 
	// 	{
	// 		.offset = {0, 0}, 
	// 		.extent = ref->swapchain.extent
	// 	},
	// 	.layerCount = 1,
	// 	.colorAttachmentCount = ARR_LEN(colorAttachmentsInfos),
	// 	.pColorAttachments = colorAttachmentsInfos,

	// 	.pDepthAttachment = &depthAttachmentInfo,
	// };

	// vkCmdBeginRendering(frame->commandBuffer, &renderingInfo);

	VkViewport viewPort = {.x = 0,
		.y = 0,
		.width = ref->swapchain.extent.width,
		.height = ref->swapchain.extent.height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f};

	vkCmdSetViewport(frame->commandBuffer, 0, 1, &viewPort);

	VkRect2D scissor = {.extent = ref->swapchain.extent, .offset = {0, 0}};
	vkCmdSetScissor(frame->commandBuffer, 0, 1, &scissor);
	
	vkCmdBindPipeline(
		frame->commandBuffer, 
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		ref->pipeline_grid.handle
	);


	VkDeviceSize offset = 0;


	VkDescriptorSet dset[] = {
		ref->sets.set_global[frame_index],

	};



	vkCmdBindDescriptorSets(
		frame->commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS, 
		ref->pipeline_grid.layout, 0,
		ARR_LEN(dset), dset, 0, NULL
	);



	vkCmdDraw(frame->commandBuffer, 3, 1, 0, 0);


	// vkCmdEndRendering(frame->commandBuffer);
}

void Mess_Proc(struct Mess* ref){

	
	int running = 1;

	ref->time_start = Platform_GetTime(ref->ref_platform);

	while (running) {

		double currentTime = Platform_GetTime(ref->ref_platform);
		double dt = (float)(currentTime - 	ref->time_last);
		ref->time_last = currentTime;
		ref->time_delta = dt;

		Platform_PollEvents(ref->ref_platform);

		Input_Update(ref->ref_input,ref->ref_backend, dt);


		Mess_Update(ref);
		
		Mess_Draw(ref);
		// Scene_Update(&app_info.scene, dt);

		// Renderer_Render(
		// 	&app_info.renderer, currentTime, dt);

		if (Platform_ShouldCloseWindow(ref->ref_platform)) {
			running = 0;
		}
	}
}

void Mess_Update(struct Mess* ref){
	update_keys(ref,  ref->time_delta);
	
	update_camera(ref,  ref->time_delta);

 // printf("inv_view[3]: %+.2f %+.2f %.+2f %+.2f\n",
 //        ref->cpu_data.camera_data.inv_view[3][0],
 //        ref->cpu_data.camera_data.inv_view[3][1],
 //       ref->cpu_data.camera_data.inv_view[3][2],
 //       ref->cpu_data.camera_data.inv_view[3][3]);


 // printf("inv_proj[2][2]: %.+4f  inv_proj[3][2]: %.+4f\n",
 //     ref->cpu_data.camera_data.inv_proj[2][2],
 //     ref->cpu_data.camera_data.inv_proj[3][2]);


	ref->cpu_data.global_data.time_total = ref->time_last;
}
static void update_keys(
	struct Mess* ref,
	float dt
) {
	struct CameraData* cam = &ref->cpu_data.camera_data;

	struct Input_State * input = ref->ref_input;
	struct Platform_State * platform = ref->ref_platform;
	

	if(Input_KeyHeldTime(input, GLFW_KEY_ESCAPE) > 1.0)
	// if(Input_IsKeyPressed(scene->ref_input, GLFW_KEY_ESCAPE))
	// if (scene->ref_input->keys[GLFW_KEY_ESCAPE] == GLFW_PRESS)
	{
		
	
		Platform_SetShouldCloseWindow(platform,true);
	}


	if(Input_IsKeyPressed(input, GLFW_KEY_LEFT_ALT)){

		Platform_ToggleCursor(platform);	
		
	}

	

	float cameraSpeed = 2.5 * dt;
	if(Input_IsKeyDown(input, GLFW_KEY_W)){
//	if (scene->ref_input->keys[GLFW_KEY_W] == GLFW_PRESS) {

		glm_vec3_muladds(
			cam->front, 
			cameraSpeed, 
			cam->pos
		);
	}
if(Input_IsKeyDown(input, GLFW_KEY_S)){
//	if (input->keys[GLFW_KEY_S] == GLFW_PRESS) {
		glm_vec3_mulsubs(
			cam->front, cameraSpeed, cam->pos);
	}
	if(Input_IsKeyDown(input, GLFW_KEY_A)){
//	if (input->keys[GLFW_KEY_A] == GLFW_PRESS) {
		vec3 temp;
		glm_vec3_crossn(cam->front, cam->up, temp);
		glm_vec3_mulsubs(temp, cameraSpeed, cam->pos);
	}
	if(Input_IsKeyDown(input, GLFW_KEY_D)){
//	if (input->keys[GLFW_KEY_D] == GLFW_PRESS) {
		vec3 temp;
		glm_vec3_crossn(cam->front, cam->up, temp);
		glm_vec3_muladds(temp, cameraSpeed, cam->pos);
	}

	if(Input_IsKeyDown(input, GLFW_KEY_SPACE)){
	// if (input->keys[GLFW_KEY_SPACE] == GLFW_PRESS) {

		glm_vec3_mulsubs(
			cam->up, cameraSpeed, cam->pos);
	}

	// printf("cam pos: %.2f %.2f %.2f\n", cam->pos[0], cam->pos[1], cam->pos[2]);
}


static void update_camera(
	struct Mess* ref,  float dt
) {

	
	struct CameraData* cam = &ref->cpu_data.camera_data;


	if(ref->ref_platform->cursor_state != GLFW_CURSOR_NORMAL)
	{
		cam->yaw -= ref->ref_input->mouseDeltaX;

		cam->pitch += ref->ref_input->mouseDeltaY;
	}
	
	if (cam->pitch > 89.0f) {
		cam->pitch = 89.0f;
	}
	if (cam->pitch < -89.0f) {
		cam->pitch = -89.0f;
	}



	vec3 front;

	front[0] = cos(glm_rad(cam->yaw)) *
			   cos(glm_rad(cam->pitch));
	front[1] = sin(glm_rad(cam->pitch));
	front[2] = sin(glm_rad(cam->yaw)) *
			   cos(glm_rad(cam->pitch));

	GLM_VEC3_COPY(cam->front, front);

	vec3 cameraCenter;
	
	glm_vec3_add(
		cam->pos, 
		cam->front, 
		cameraCenter
	);
	
	glm_lookat_rh(
		cam->pos, 
		cameraCenter, 
		cam->up,
		cam->view
	);

	glm_mat4_mul(
		cam->proj, 
		cam->view,
		cam->view_proj
	);

	glm_mat4_copy(cam->view, cam->inv_view);
	glm_mat4_inv(cam->inv_view, cam->inv_view);
	
	glm_mat4_copy(cam->proj, cam->inv_proj);
	glm_mat4_inv(cam->inv_proj, cam->inv_proj);
}
