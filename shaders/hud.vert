#version 450

#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader/common.h"
#include "shader/binding_hud.h"

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec4 in_color;

layout(location = 0) out vec2 out_texCoord;
layout(location = 1) out vec4 out_color;





layout(set = HUD_DESC_SET_GLOBALS, binding = HUD_BINDING_GLOBAL_GLOBAL) uniform Global
{
    GlobalData global;
};

layout(set = HUD_DESC_SET_GLOBALS, binding = HUD_BINDING_GLOBAL_CAMERA) uniform Global_Camera
{
    Camera2DData camera;
};

layout(set = HUD_DESC_SET_INSTANCES, binding = 0) readonly buffer Instance
{
    InstanceData instance[];
};

layout(set = HUD_DESC_SET_MATERIALS, binding = 0) readonly buffer Materials
{
    MaterialData material[];
};




// layout(location = 2) out vec2 out_fragPos;



void main() {
	
	vec4 tpos = models[gl_InstanceIndex] * vec4(in_position, 0.0 , 1.0);

	gl_Position = projection * view * tpos;
	
	out_texCoord = in_texCoord;
	
	out_color = in_color ;
	



}
