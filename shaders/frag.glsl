#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"


layout(location = 0) in vec2 texCoord;

layout(location = 1) in vec3 normal;

layout(location = 2) in vec3 fragPos;

// layout(location = 3) in vec4 in_objectId;

layout(location = 3) flat in uint in_objectId;

layout(location = 0) out vec4 outColor;

layout(location = 1) out uint outID;


layout(binding = BINDING_FRAG_UBO_Lights) uniform Light
{
  vec4 lightPos; 
  vec4 viewPos; 
  vec4 lightColor;

};

layout(binding = BINDING_FRAG_SAMPLER) uniform sampler2D  tex[];

layout(push_constant) uniform Push {
    int textureIndex;
	int hasColor;
	int selectedId;
} pc;


void main() {
	// // vec4 texColor = mix(texture(tex, texCoord), fragColor, 0.1);

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
    vec4 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec4 norm = vec4(normalize(normal),0.0);
    vec4 lightDir = normalize(lightPos - vec4(fragPos,0.0));
    float diff = max(dot(norm, lightDir),0.0);
    vec4 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec4 viewDir = normalize(viewPos - - vec4(fragPos,0.0));
    vec4 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor;  
        
    vec4 result = (ambient + diffuse + specular) * texColor;

    outColor = vec4(result);
	
	
	

}

