#ifndef __SHADERS__
#pragma once
#include <stdint.h>
#include "cglm/cam.h"
#include "cglm/cglm.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/util.h"
#endif


#define DESC_SET_GLOBALS 0
#define DESC_SET_INSTANCES 1
#define DESC_SET_MATERIALS 2
#define DESC_SET_TEXTURES 3



#define MAX_MATERIALS 1
#define MAX_TEXTURES 1
#define MAX_INSTANCES 1024
#define MAX_LIGHTS 1;


#ifndef __SHADERS__

typedef uint32_t uint;

#endif

struct Instance_ssbo
{
	mat4 model;
	vec4 color;
	float color_factor;
	uint tex_idx;
	
};
struct Camera_o{
	mat4 view;
	mat4 model;
	vec3 pos;
	vec3 view_dir;
	vec3 view_pos;
};

struct Light_o{
	vec3 pos;
	vec3 view_dir;
	vec3 view_pos;
	vec4 color;
	float color_factor;
};

struct Material_ssbo{
	vec4 color;
	float color_factor;
	uint texture_idx;
};

struct Global_ubo{
	mat4 view;
	mat4 proj;

	float time_total;
	float time_delta;

	uint frame_cnt;

#ifndef __SHADERS__
	struct 
#endif	
	Camera_o cam;


#ifndef __SHADERS__
	struct 
#endif	
	Light_o light;


	uint instance_cnt;
};