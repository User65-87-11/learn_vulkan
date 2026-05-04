#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"



layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in flat uint instance_id;

//to the first color attachment
layout(location = 0) out vec4 out_color;


layout(std430, set = DESC_SET_INSTANCES, binding = 0) readonly buffer Instance
{
	Instance_ssbo inst[];
};

layout(std430, set = DESC_SET_MATERIALS, binding = 0) readonly buffer Materials
{
	Material_ssbo material[];
};

layout(set = DESC_SET_TEXTURES, binding = 0) uniform sampler2D  tex[MAX_TEXTURES];




void main() {
	
	uint idx = instance_id;

	uint tex_idx = inst[idx].tex_idx;

	vec4 texColor = texture(tex[tex_idx], texCoord);

	vec4 inst_color = inst[idx].color;

	float color_factor = inst[idx].color_factor;

	vec4 finalColor = mix(texColor, inst_color, color_factor);

	out_color = finalColor;



}

