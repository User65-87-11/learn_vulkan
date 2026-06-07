#ifndef __SHADERS__
#pragma once

#include <stdint.h>
#include "cglm/cam.h"
#include "cglm/cglm.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/util.h"


#include "common.h"

typedef uint32_t uint;

#endif

#define UNSET_VALUE 0xffffffff 
#define UNSET_BYTE  0xff




#define DESC_SET_GLOBALS 0
#define DESC_SET_INSTANCES 1
#define DESC_SET_MATERIALS 2
#define DESC_SET_TEXTURES 3
#define DESC_SET_SAMPLER 4
#define DESC_SET_NOISE 5
#define DESC_SET_STORAGE_IMAGE 6



#define BINDING_GLOBAL_GLOBAL 0
#define BINDING_GLOBAL_CAMERA 1
#define BINDING_GLOBAL_LIGHT 2

#define MAX_MATERIALS 1
#define MAX_TEXTURES 1
#define MAX_INSTANCES 1024
#define MAX_LIGHTS 1


#define MAX_NODES 1024


#ifndef __SHADERS__
struct Scene_Node{
	mat4 matrix;
	vec3 translation;
	//quaternion
	vec4 rotation;
	vec3 scale;

	uint32_t has_matrix;
	uint16_t _child_nodes_ids[MAX_STATIC_CHILD_NODES];
	uint16_t * child_nodes_ids;
	uint32_t child_cnt;
	uint32_t dynamic_nodes;
	
};

struct Scene_Root{
	uint32_t * nodes;
	uint32_t node_cnt;
};
#endif

struct InstanceData
{
	#ifdef __SHADERS__
    mat4 model;
    #else
    vec4 model[4];
    #endif

    uint material_idx;
 

    
}
#ifndef __SHADERS__
	__attribute__((aligned(32)))
#endif
;

struct MaterialData {

	// w = factor
	vec4 base_color_factor;

	uint base_color_texture_idx;

	float metallic_factor;

	float roughness_factor;

	uint metallic_roughness_texture_idx;
};



struct CameraData {
	#ifdef __SHADERS__
    mat4 view;
    mat4 proj;
    mat4 view_proj;
    #else
    vec4 view[4];
    vec4 proj[4];
    vec4 view_proj[4];
    #endif
    vec3 pos;
    float yaw;
    
    vec3 front;
    float pitch;
    
    vec3 up;
    float fov;

    float near_plane;
    float far_plane;
    float aspect_ratio;

    float ortho_size;
}
#ifndef __SHADERS__
	__attribute__((aligned(32)))
#endif
;



struct LightData {
    vec3 pos;
    vec3 view_dir;
    vec3 view_pos;
    vec3 color;
    float color_factor;
};





struct GlobalData {
	
    vec2 framebuffer_size;

	float time_total;
    float time_delta;

    uint frame_cnt;
    uint instance_cnt; 

} ;

#ifndef __SHADERS__
struct CommonData{
	struct CameraData camera;
	struct LightData light;
	struct GlobalData global;
};
#endif

#ifndef __SHADERS__
struct Mesh {
	uint32_t vertex_offset;
	uint32_t index_offset;

	// uint32_t vertex_count;
	uint32_t index_count;

	uint32_t instance_cnt;
	uint32_t instance_offset;
};
#endif
