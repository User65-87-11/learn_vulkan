#version 430 core

layout (location = 0) in vec3 vpos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vcol;
layout (location = 3) in vec2 vTexCoord;

 


layout (location = 10) in mat4 model;
 // +11,12,13
 

//uniform mat4 projection;


out vec4 frag_color;
out vec3 frag_pos;  
out vec2 frag_text_coord;  
out vec3 normal;
 


layout (std140, binding = 0) uniform Matrices
{
    mat4 pv;

};


 

void main()
{


   	gl_Position = pv * model* vec4(vpos  ,1.0) ;
  
	frag_color = vcol;
	frag_pos = vec3(model * vec4(vpos, 1.0));
	normal = vNormal;
	frag_text_coord = vTexCoord;

}