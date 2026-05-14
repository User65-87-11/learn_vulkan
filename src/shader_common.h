#ifndef __SHADERS__
#pragma once

#include <stdint.h>
#include "cglm/cam.h"
#include "cglm/cglm.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/util.h"

typedef uint32_t uint;

#endif

#define UNSET_VALUE 0xffffffff 
#define UNSET_BYTE  0xff




#define DESC_SET_GLOBALS 0
#define DESC_SET_INSTANCES 1
#define DESC_SET_MATERIALS 2
#define DESC_SET_TEXTURES 3

#define BINDING_GLOBAL_GLOBAL 0
#define BINDING_GLOBAL_CAMERA 1
#define BINDING_GLOBAL_LIGHT 2

#define MAX_MATERIALS 1
#define MAX_TEXTURES 1
#define MAX_INSTANCES 1024
#define MAX_LIGHTS 1


/*

st140 (max)16
float = 4
float[1] = 16
float[4]
	== [flaot + 12]+[flaot + 12]+[flaot + 12]+[flaot + 12]

vec4 =16 
	== float + float + float  + float
	!= flaot[4]
mat4 = 16
	== vec4[4]
	== 16*float



std430
float = 4
float[1] = 4
vec2 = 8
vec3-4 = 16
mat4 = 16

 */

/*
	vec3 positions[3] = vec3[](
            vec3(-0.6, -0.4, -4.0),
            vec3(0.6, -0.4, -4.0),
            vec3(0.0, 0.6, -4.0)
        );
 */


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

///generate usign Clang AST
