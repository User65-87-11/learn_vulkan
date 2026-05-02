#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"
#include "shader_inc2.glsl"

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
#define BINDING_3D_SAMPLERS 0
#define BINDING_2D_SAMPLERS 1
#define BINDING_VIEW_PROJECTIONS_3D 2
#define BINDING_VIEW_PROJECTIONS_2D 3
#define BINDING_VIEW_PROJECTIONS_LIGHT 4
#define BINDING_MODELS 5
#define BINDING_DIRECTIONAL_LIGHTS 6
#define BINDING_OBJECT_IDS 7
#define BINDING_2D_COLORS 8
*/




layout(binding = BINDING_2D_SAMPLERS) uniform sampler2D  tex[];

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

	// outColor = vec4(1.0,1.0,1.0,1.0);

}

