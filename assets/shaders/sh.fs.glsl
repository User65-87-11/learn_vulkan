#version 430 core

// in vec4 frag_color;
// in vec3 normal;  
// in vec3 frag_pos;  
// in vec2 frag_text_coord;

out vec4 out_color;


 
// uniform vec3 lightPos;

// uniform sampler2D Texture;

void main()
{    
 	//vec3 norm = normalize(normal);

	//out_color = texture(Texture, frag_text_coord);

	out_color = vec4(1.0,1.0,0.5,1.0);
}