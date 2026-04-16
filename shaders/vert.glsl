#version 450

struct ModelData {
	
    mat4 model;
	uint objectId;
	 
};



layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_texCoord;

layout(binding = 0) uniform Matrices
{

  mat4 view;
  mat4 projection;
};



layout(std430, binding = 1) readonly buffer  Models
{

	ModelData modelData[];

};
 


layout(location = 0) out vec2 out_texCoord;

layout(location = 1) out vec3 out_norm;

layout(location = 2) out vec3 out_fragPos;

layout(location = 3) flat out uint out_objectId;

void main() {
	
	vec4 tpos = modelData[gl_InstanceIndex].model * vec4(in_position, 1.0);

	gl_Position = projection * view * tpos;
	
	out_texCoord = in_texCoord;
	
	out_norm = in_norm;
	
	out_fragPos = tpos.xyz;


	out_objectId = modelData[gl_InstanceIndex].objectId;
	// float colId = modelData[gl_InstanceIndex].objectId;
	// out_objectId = modelData[gl_InstanceIndex].objectId;
	// out_fragPos = in_position;
}
