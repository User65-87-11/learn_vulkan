#version 450
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_texCoord;



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



layout(binding = BINDING_VIEW_PROJECTIONS_3D) uniform Matrices
{
  mat4 view;
  mat4 projection;
};

layout(binding = BINDING_VIEW_PROJECTIONS_LIGHT) uniform LightMatrix
{
  mat4 light_view;
  mat4 light_proj;
};


layout(std430, binding = BINDING_MODELS_3D) readonly buffer  Models
{
	mat4 model[];
};

layout(std430, binding = BINDING_OBJECT_IDS) readonly buffer  ObjectIds
{
	uint objectId[];
};
 


layout(location = 0) out vec2 out_texCoord;

layout(location = 1) out vec3 out_norm;

layout(location = 2) out vec3 out_fragPos;

layout(location = 3) flat out uint out_objectId;


layout(location = 5) out vec4 fragPosLightSpace;

layout(location = 4) out float out_fog_depth;

void main() {
	
	vec4 world_pos = model[gl_InstanceIndex] * vec4(in_position, 1.0);

	vec4 viewPos = view * world_pos;

	out_fog_depth = -viewPos.z;

	gl_Position = projection * view * world_pos;
	
	out_texCoord = in_texCoord;
	
	out_norm = in_norm;
	
	out_fragPos = world_pos.xyz;


	fragPosLightSpace = light_proj * light_view * world_pos;

	out_objectId = objectId[gl_InstanceIndex];
	// float colId = modelData[gl_InstanceIndex].objectId;
	// out_objectId = modelData[gl_InstanceIndex].objectId;
	// out_fragPos = in_position;
}
