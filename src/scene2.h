#pragma once
#include <vulkan/vulkan_core.h>
#include "src/resource.h"
#include "util/common.h"
#include "shader_common.h"
#include "cglm/types.h"

#define MAX_ENTITIES 1024
#define MAX_MESHES 512

/*

So scene has SSBO UBO and renderer has their buffers?

 */


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

	uint32_t vertex_count;
	uint32_t index_count;

	uint32_t material_index;
	uint32_t instance_index;
};

struct Entity {
	char name[64];
	uint32_t mesh_index;
	uint32_t material_index;
	bool is_visible;
};

struct Scene {
	struct Entity entities[MAX_ENTITIES];
	uint32_t entities_count;

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
};

// struct Scene * Scene_Get();

void Scene_Init(struct Scene* scene, float aspect_ratio);

void Scene_Update(struct Scene* scene, float dt);

struct Entity* Scene_NewEntity(struct Scene* scene);

struct Entity* Scene_GetEntity(struct Scene* scene, uint32_t position);

struct Mesh* Scene_NewMesh(struct Scene* scene);

struct Mesh* Scene_GetMesh(struct Scene* scene, uint32_t position);

struct MaterialData* Scene_NewMaterial(struct Scene* scene);

struct MaterialData* Scene_GetMaterial(struct Scene* scene, uint32_t position);

struct InstanceData* Scene_NewInstanceData(struct Scene* scene);

struct InstanceData* Scene_GeInstanceData(
	struct Scene* scene, uint32_t position);

// struct Texture *  Scene_GetTexture(struct Scene * scene,  uint32_t position);
