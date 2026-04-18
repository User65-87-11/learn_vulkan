#version 450
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_texCoord;



layout(binding = BINDING_VERT_1_UBO_ViewProjection) uniform Matrices
{
  mat4 view;
  mat4 projection;
};






layout(std430, binding = BINDING_VERT_1_SSBO_Models) readonly buffer  Models
{
	mat4 model[];
};

layout(std430, binding = BINDING_VERT_1_SSBO_ObjectIDS) readonly buffer  ObjectIds
{
	uint objectId[];
};
 


layout(location = 0) out vec2 out_texCoord;

layout(location = 1) out vec3 out_norm;

layout(location = 2) out vec3 out_fragPos;

layout(location = 3) flat out uint out_objectId;

void main() {
	
	vec4 tpos = model[gl_InstanceIndex] * vec4(in_position, 1.0);

	gl_Position = projection * view * tpos;
	
	out_texCoord = in_texCoord;
	
	out_norm = in_norm;
	
	out_fragPos = tpos.xyz;


	out_objectId = objectId[gl_InstanceIndex];
	// float colId = modelData[gl_InstanceIndex].objectId;
	// out_objectId = modelData[gl_InstanceIndex].objectId;
	// out_fragPos = in_position;
}
