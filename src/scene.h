#pragma once
#include <vulkan/vulkan_core.h>
#include "device2.h"
#include "input.h"
#include "resource.h"
#include "util/common.h"
#include "shader_common.h"
#include "cglm/types.h"

#define MAX_ENTITIES 1024
#define MAX_MESHES 512

/*

So scene has SSBO UBO and renderer has their buffers?

 */

struct Scene_Data{

	struct GlobalData global_data;
	struct CameraData camera_data;
	struct LightData light_data;
	
	struct InstanceData instance_data[MAX_INSTANCES];
	
	struct MaterialData material_data[MAX_MATERIALS];
};

struct SceneNode{
   	uint32_t parent;
	uint32_t* children;
	uint32_t child_count;
	uint32_t next_sibling;
   
	//---
	vec3 translation;
	vec4 rotation;
	vec3 scale;
   
	mat4 matrix;
	uint8_t has_matrix;
   
	uint32_t mesh;
};

struct Mesh {
	uint32_t vertex_offset;
	uint32_t index_offset;

	// uint32_t vertex_count;
	uint32_t index_count;

	uint32_t instance_cnt;
	uint32_t instance_offset;
};



struct Scene {
	struct Device_State * ref_device;
	struct Input_State * ref_input;
	struct Platform_State * ref_platform;
	

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


	struct Scene_Data * data;
};

struct Scene_Info{
	struct Device_State * ref_device;
	struct Input_State * ref_input;
	struct Platform_State * ref_platform;

		
	
	uint32_t width;
	uint32_t height;
};

// struct Scene * Scene_Get();

void Scene_Init(struct Scene_Info * info, struct Scene* scene);

void Scene_Destroy(struct Scene* scene);

void Scene_Update(struct Scene* scene, float dt);


struct Mesh* Scene_NewMesh(struct Scene* scene);

struct Mesh* Scene_GetMesh(struct Scene* scene, uint32_t position);

struct MaterialData* Scene_NewMaterial(struct Scene* scene);

struct MaterialData* Scene_GetMaterial(struct Scene* scene, uint32_t position);

struct InstanceData* Scene_NewInstanceData(struct Scene* scene);

struct InstanceData* Scene_GeInstanceData(
	struct Scene* scene, uint32_t position);

// struct Texture *  Scene_GetTexture(struct Scene * scene,  uint32_t position);



void Scene_callback_FrameBuffer_Resize(void * window,uint32_t w,uint32_t h);
