#version 450

#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texCoord;

layout(location = 0) out vec2 out_texCoord;
layout(location = 1) out vec4 out_color;


/*
#define BINDING_VERT_UBO_ViewProjection  0
#define BINDING_VERT_SSBO_Models   10
#define BINDING_VERT_SSBO_ObjectIDS   11
#define BINDING_VERT_SSBO_Colors   12

#define BINDING_FRAG_UBO_Lights   30
#define BINDING_FRAG_SAMPLER  40

*/

layout(binding = BINDING_VERT_UBO_ViewProjection) uniform Matrices
{
  mat4 view;
  mat4 projection;
};


layout(std430, binding = BINDING_VERT_SSBO_Models) readonly buffer  Models
{
	mat4 models[];
};


layout(std430, binding = BINDING_VERT_SSBO_Colors) readonly buffer  Colors
{
	vec4 colors[];
 
};




// layout(location = 2) out vec2 out_fragPos;



void main() {
	
	vec4 tpos = models[gl_InstanceIndex] * vec4(in_position, 0.0 , 1.0);

	gl_Position = projection * view * tpos;
	
	out_texCoord = in_texCoord;
	

	out_color = colors[gl_InstanceIndex] ;
	
	// out_norm = in_norm;
	
	// out_fragPos = tpos.xy;


}
