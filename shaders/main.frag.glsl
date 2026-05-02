#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"
#include "shader_inc2.glsl"


layout(location = 0) in vec2 texCoord;

layout(location = 1) in vec3 normal;

layout(location = 2) in vec3 fragPos;

// layout(location = 3) in vec4 in_objectId;

// layout(location = 3) flat in uint in_objectId;


layout(location = 4) in float in_fog_depth;


layout(location = 5) in vec4 fragPosLightSpace;

// layout(location = 6) in vec4 in_color;

layout(location = 0) out vec4 outColor;

layout(location = 1) out uint outID;


/*
struct InstanceHUD{
    vec2 position;
    vec2 size;
    vec4 color;
    uint material_id;
    uint object_id;
	uint flags;
};

struct InstanceMesh{
	mat4 model;
	vec4 color;
	uint object_id;
	uint material_id;
	uint flags;
	//add padding
};
struct Material{
	vec4 baseColorFactor;
    float metallicFactor;
    float roughnessFactor;
	//add padding
};


struct GlobalData {
	mat4 view;
	mat4 proj;
	vec2 screenSize;
	float time;
	float deltaTime;
};

struct CameraData {
    mat4 view;
    mat4 proj;
    vec4 position;
};


struct LightData {
	mat4 view;
	mat4 proj;
    vec4 direction;
    vec4 color;
    float intensity;
};

*/


/*
#define DESC_SET_GLOBAL 0
#define BIND_GLOBAL_GLOBAL 0
#define BIND_GLOBAL_CAMERA 1
#define BIND_GLOBAL_LIGHT 2

#define DESC_SET_INSTANCE 1
#define BIND_INSTANCE_MESH 0
#define BIND_INSTANCE_HUD 0

#define DESC_SET_MATERIAL 2


gl_InstanceIndex
*/

layout(std140, set = DESC_SET_GLOBAL, binding = BIND_GLOBAL_GLOBAL) uniform GlobalData_Global
{
	GlobalData global;
};
layout(std140, set = DESC_SET_GLOBAL, binding = BIND_GLOBAL_CAMERA) uniform GlobalData_Camera
{
	CameraData global_camera;
};

layout(std140, set = DESC_SET_GLOBAL, binding = BIND_GLOBAL_LIGHT) uniform GlobalData_Light
{
	LightData global_light;
};


layout(std430,set = DESC_SET_INSTANCE, binding = BIND_INSTANCE_MESH) readonly buffer   InstanceData_MESH
{
	InstanceMesh inst_mesh[];
};

layout(std430, set = DESC_SET_INSTANCE, binding = BIND_INSTANCE_HUD) readonly buffer   InstanceData_HUD
{
	InstanceHUD inst_hud[];
};


layout(std430,set = DESC_SET_MATERIAL, binding = 0) readonly buffer   MaterialData
{
	Material materials[];
};



layout(set = DESC_SET_SAMPLERS, binding = BIND_SAMPLER_3D) uniform sampler2D  tex_3d[];

layout(set = DESC_SET_SAMPLERS, binding = BIND_SAMPLER_2D) uniform sampler2D  tex_hud[];

layout(binding = BINDING_3D_SAMPLERS_SHADOW) uniform sampler2DShadow   shadowMap;

// layout(push_constant) uniform Push {
//     int textureIndex;
// 	int hasColor;
// 	int selectedId;
// } pc;

float shadowFactor(vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    

    projCoords.xy = clamp(projCoords.xy, 0.0, 1.0);
    
 
    if (projCoords.z < 0.0 || projCoords.z > 1.0) return 1.0;

	float bias = 0.005;

    return texture(shadowMap, vec3(projCoords.xy, projCoords.z - bias));
}
vec3 calcLight(vec4 texColor,float shadow){

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * global_light.lightColor.rgb;
  	
	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(global_light.lightPos.xyz - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * global_light.lightColor.rgb;
    
	// specular
	float specularStrength = 0.5;

	vec3 viewDir = normalize(global_light.view.xyz - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * global_light.lightColor.rgb;
        
	//	
	vec3 lighting = (ambient + (diffuse + specular) * shadow) * texColor.rgb;
	return lighting;
}


void main() {
	// // vec4 texColor = mix(texture(tex, texCoord), fragColor, 0.1);

	InstanceMesh * mesh = inst_mesh[gl_InstanceIndex];

	float shadow = shadowFactor(fragPosLightSpace);


	outID = mesh.object_id;
	vec4 texColor = texture(tex[mesh.material_id], texCoord);

	if(mesh.flags & GM_FLAGS_SELECTED){
		texColor = global_light.lightColor; 
	}

	if(pc.hasColor != 0)
	{
		texColor = in_color;
	}

    // outColor = texColor;


	
	vec3 lighting = calcLight(texColor,shadow);

	vec3 fog_color = vec3(0.0, 0.0, 0.0);

	float fogFactor = clamp(in_fog_depth / 20.0, 0.0, 1.0);

	vec3 finalColor = mix(lighting, fog_color, fogFactor);

	outColor = vec4(finalColor, texColor.a);

	
	

}

