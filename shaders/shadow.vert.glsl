#version 450
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"
#include "shader_inc2.glsl"

layout(location = 0) in vec3 in_position;


/*
#define BINDING_3D_SAMPLERS 0
#define BINDING_2D_SAMPLERS 1
#define BINDING_VIEW_PROJECTIONS_3D 2
#define BINDING_VIEW_PROJECTIONS_2D 3
#define BINDING_VIEW_PROJECTIONS_LIGHT 4
#define BINDING_MODELS_3D 5
#define BINDING_MODELS_2D 6
#define BINDING_DIRECTIONAL_LIGHTS 7
#define BINDING_OBJECT_IDS 8
#define BINDING_2D_COLORS 9
*/



layout(binding = BINDING_VIEW_PROJECTIONS_LIGHT) uniform LightVP
{
	mat4 lightView;
	mat4 projection;
};

layout(std430, binding = BINDING_MODELS_3D) readonly buffer  Models
{
	mat4 model[];
};

void main(){

	vec4 tpos = model[gl_InstanceIndex] * vec4(in_position, 1.0);

	gl_Position = projection * lightView * tpos;

	//depthBiasEnable = VK_TRUE
}