#pragma once
#include "cglm/types.h"
#include <stdint.h>

struct GLTFScene {
	
	uint32_t* root_nodes;
	uint32_t root_node_count;
};

struct GLTFNode {
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

struct GLTFMaterial {
	vec4 base_color_factor;

	uint32_t base_color_texture;

	float metallic_factor;

	float roughness_factor;

	uint32_t metallic_roughness_texture;
};

struct GLTFMesh {
	struct GLTFPrimitive* primitives;
	uint32_t primitive_count;
};
// = InstanceData
struct GLTFPrimitive {
	float* positions;
	float* normals;
	float* texcoords;
	float* tangents;

	uint32_t vertex_count;

	uint32_t* indices;
	uint32_t index_count;

	uint32_t material;
};

struct GLTFImage {
	uint8_t* pixels;
	uint32_t width;
	uint32_t height;
	uint32_t channels;
};

struct GLTFAsset {
	struct GLTFScene* scenes;
	uint32_t scene_count;

	struct GLTFNode* nodes;
	uint32_t node_count;

	struct GLTFMesh* meshes;
	uint32_t mesh_count;

	struct GLTFMaterial* materials;
	uint32_t material_count;

	struct GLTFImage* images;
	uint32_t image_count;
};

struct GLTFAsset* gltf_AllocAsset();

struct GLTFScene* gltf_AllocScenes(
	struct GLTFAsset* asset, 
	uint32_t num
);

struct GLTFScene* gltf_SetScene(
	struct GLTFAsset* asset, 
	struct GLTFScene* scene
);

struct GLTFMesh* gltf_AllocMeshes(
	struct GLTFAsset* asset, 
	uint32_t num
);

void gltf_SetMesh(
	struct GLTFAsset* asset, 
	struct GLTFMesh* mesh
);

struct GLTFMaterial* gltf_AllocMaterials(
	struct GLTFAsset* asset, 
	uint32_t num
);

void gltf_SetMaterial(
	struct GLTFAsset* asset, 
	struct GLTFMaterial* mesh
);

struct GLTFImage* gltf_AllocImages(
	struct GLTFAsset* asset, 
	uint32_t num
);

void gltf_SetImage(
	struct GLTFAsset* asset, 
	struct GLTFImage* image
);

struct GLTFNode* gltf_AllocNodes(
	struct GLTFAsset* asset, 
	uint32_t num
);

void gltf_SetNodes(
	struct GLTFAsset* asset, 
	struct GLTFNode* node
);

struct GLTFPrimitive* gltf_AllocPrimitives(
	struct GLTFAsset* asset, 
	uint32_t num
);

void gltf_SetPrimitives(
	struct GLTFAsset* asset, 
	struct GLTFPrimitive* primitive
);

void gltf_FreeAsset(struct GLTFAsset* asset);

void gltf_FreeScenes(struct GLTFScene* scenes);


void gltf_Load(char * path, struct GLTFAsset* asset);
