#version 450
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"
#include "shader_inc2.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_texCoord;

layout(location = 0) out vec2 out_texCoord;

layout(location = 1) out vec3 out_norm;

layout(location = 2) out vec3 out_fragPos;

// layout(location = 3) flat out uint out_objectId;



layout(location = 4) out float out_fog_depth;

layout(location = 5) out vec4 fragPosLightSpace;

// layout(location = 6) out vec4 out_color;



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
	//add paddng
};

struct CameraData {
    mat4 view;
    mat4 proj;
    vec4 position;
	//add padding
};


struct LightData {
    vec4 direction;
    vec4 color;
    float intensity;
	//add padding
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

layout(std430,set = DESC_SET_INSTANCE, binding = BIND_INSTANCE_HUD) readonly buffer   InstanceData_HUD
{
	InstanceHUD inst_hud[];
};



/*

gl_InstanceID 
gl_InstanceIndex 

gl_VertexID 
gl_VertexIndex 
*/



void main() {
	
	vec4 world_pos = model[gl_InstanceIndex] * vec4(in_position, 1.0);

	vec4 viewPos = global_camera.view * world_pos;

	out_fog_depth = -viewPos.z;

	gl_Position = global_camera.proj * viewPos;
	
	out_texCoord = in_texCoord;
	
	out_norm = in_norm;
	
	out_fragPos = world_pos.xyz;


	fragPosLightSpace = global_light.proj * global_light.view * world_pos;

	// out_objectId = inst_mesh[gl_InstanceIndex].object_id;

	// out_color = inst_mesh[gl_InstanceIndex];
	// float colId = modelData[gl_InstanceIndex].objectId;
	// out_objectId = modelData[gl_InstanceIndex].objectId;
	// out_fragPos = in_position;
}
