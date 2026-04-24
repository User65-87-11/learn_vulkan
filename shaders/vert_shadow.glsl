#version 450
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_norm;


layout(binding = BINDING_FRAG_3_UBO_Lights) uniform LightVP
{
	mat4 lightView;
	mat4 projection;
};

layout(std430, binding = BINDING_VERT_3_SSBO_Models) readonly buffer  Models
{
	mat4 model[];
};

void main(){

	vec4 tpos = model[gl_InstanceIndex] * vec4(in_position, 1.0);

	gl_Position = projection * lightView * tpos;

	//depthBiasEnable = VK_TRUE
}