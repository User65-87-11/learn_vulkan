#version 450
 

layout(location = 0) in vec2 texCoord;

layout(location = 1) in vec3 normal;

layout(location = 2) in vec3 fragPos;

layout(location = 0) out vec4 outColor;


layout(binding = 1) uniform Light
{
  vec4 lightPos; 
  vec4 viewPos; 
  vec4 lightColor;

};

layout(binding = 2) uniform sampler2D  tex;




void main() {
	// // vec4 texColor = mix(texture(tex, texCoord), fragColor, 0.1);

	 vec4 texColor = texture(tex, texCoord);

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

