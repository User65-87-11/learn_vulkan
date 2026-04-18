#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"

layout(location = 0) in vec2 texCoord;

layout(location = 1) in vec4 fragColor;

// layout(location = 3) in vec4 in_objectId;

// layout(location = 3) flat in uint in_objectId;

layout(location = 0) out vec4 outColor;

// layout(location = 1) out uint outID;


// layout(binding = 3) uniform Light
// {
//   vec4 lightPos; 
//   vec4 viewPos; 
//   vec4 lightColor;

// };

/*
#define BINDING_VERT_UBO_ViewProjection  0
#define BINDING_VERT_SSBO_Models   10
#define BINDING_VERT_SSBO_ObjectIDS   11
#define BINDING_VERT_SSBO_Colors   12

#define BINDING_FRAG_UBO_Lights   30
#define BINDING_FRAG_SAMPLER  40

*/


layout(binding = BINDING_FRAG_SAMPLER) uniform sampler2D  tex[];

layout(push_constant) uniform Push {
    uint textureIndex;
	uint hasColor;

} pc;


void main() {
	// // vec4 texColor = mix(texture(tex, texCoord), fragColor, 0.1);

	
	vec4 texColor = texture(tex[pc.textureIndex], texCoord);

	if(pc.hasColor != 0)
	{
		texColor = fragColor;
	}

    outColor = texColor;



   
	
	
	

}

