#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"


layout(location = 0) in vec2 texCoord;

layout(location = 1) in vec3 normal;

layout(location = 2) in vec3 fragPos;

// layout(location = 3) in vec4 in_objectId;

layout(location = 3) flat in uint in_objectId;


layout(location = 4) in float in_fog_depth;


layout(location = 5) in vec4 fragPosLightSpace;

layout(location = 0) out vec4 outColor;

layout(location = 1) out uint outID;


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



layout(binding = BINDING_DIRECTIONAL_LIGHTS) uniform Light
{
  vec4 lightPos; 
  vec4 viewPos; 
  vec4 lightColor;

};

layout(binding = BINDING_3D_SAMPLERS) uniform sampler2D  tex[];

layout(binding = BINDING_3D_SAMPLERS_SHADOW) uniform sampler2DShadow   shadowMap;

layout(push_constant) uniform Push {
    int textureIndex;
	int hasColor;
	int selectedId;
} pc;

float shadowFactor(vec4 lightSpacePos)
{
       vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    
    // ✅ REMOVED: projCoords = projCoords * 0.5 + 0.5;
    // Vulkan + GLM_FORCE_DEPTH_ZERO_TO_ONE already outputs [0, 1] depth.

    // Clamp to avoid sampling outside the shadow map (prevents hard edges/artifacts)
    projCoords.xy = clamp(projCoords.xy, 0.0, 1.0);
    
    // If fragment is outside the light's view volume, it's fully lit
    if (projCoords.z < 0.0 || projCoords.z > 1.0) return 1.0;

	float bias = 0.005;

    return texture(shadowMap, vec3(projCoords.xy, projCoords.z - bias));
}


void main() {
	// // vec4 texColor = mix(texture(tex, texCoord), fragColor, 0.1);

	 float shadow = shadowFactor(fragPosLightSpace);


	outID = in_objectId;
	vec4 texColor = texture(tex[pc.textureIndex], texCoord);

	if(in_objectId == pc.selectedId){
		texColor = lightColor; 
	}

	if(pc.hasColor != 0)
	{
		texColor = lightColor;
	}

    // outColor = texColor;


	
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor.rgb;
  	
	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos.xyz - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor.rgb;
    
	// specular
	float specularStrength = 0.5;

	vec3 viewDir = normalize(viewPos.xyz - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * lightColor.rgb;
        
	//	
	vec3 lighting = (ambient + (diffuse + specular) * shadow) * texColor.rgb;

	vec3 fog_color = vec3(0.0, 0.0, 0.0);

	float fogFactor = clamp(in_fog_depth / 20.0, 0.0, 1.0);

	vec3 finalColor = mix(lighting, fog_color, fogFactor);

	outColor = vec4(finalColor, texColor.a);

	
	

}

