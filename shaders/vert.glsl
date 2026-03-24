#version 450



layout(location = 0) in vec2 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_texCoord;

layout(binding = 0) uniform Matrices
{
  mat4 model;
  mat4 view;
  mat4 projection;

};


layout(location = 0) out vec4 out_color;

layout(location = 1) out vec2 out_texCoord;


void main() {
	
	gl_Position = projection * view * model * vec4(in_position, 0.0, 1.0);

    out_color  = vec4(in_color,1.0);
	
	out_texCoord = in_texCoord;
	
}
