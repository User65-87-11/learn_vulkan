#version 450



layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texCoord;

layout(location = 0) out vec2 out_texCoord;
layout(location = 1) out vec4 out_color;

layout(binding = 0) uniform Matrices
{
  mat4 view;
  mat4 projection;
};


layout(std430, binding = 1) readonly buffer  Models
{
	mat4 models[];
};


layout(std430, binding = 2) readonly buffer  Colors
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
