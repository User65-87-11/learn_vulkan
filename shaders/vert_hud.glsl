#version 450

#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texCoord;

layout(location = 0) out vec2 out_texCoord;
layout(location = 1) out vec4 out_color;



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




layout(binding = BINDING_VIEW_PROJECTIONS_2D) uniform Matrices
{
  mat4 view;
  mat4 projection;
};


layout(std430, binding = BINDING_MODELS_2D) readonly buffer  Models
{
	mat4 models[];
};


layout(std430, binding = BINDING_COLORS) readonly buffer  Colors
{
	vec4 colors[];
 
};




// layout(location = 2) out vec2 out_fragPos;



void main() {
	
	vec4 tpos = models[gl_InstanceIndex] * vec4(in_position, 0.0 , 1.0);

	gl_Position = projection * view * tpos;
	
	out_texCoord = in_texCoord;
	

	out_color = colors[gl_InstanceIndex] ;
	


    // vec2 positions[4] = vec2[](
    //     vec2(-0.5, -0.5), // 0
    //     vec2( 0.5, -0.5), // 1
    //     vec2( 0.5,  0.5), // 2
    //     vec2(-0.5,  0.5)  // 3
    // );

    // gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);

}
