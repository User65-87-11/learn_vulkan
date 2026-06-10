#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "debug.glsl"
#include "shader/common.h"
#include "shader/binding_main.h"

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in flat uint instance_idx;

layout(location = 3) in vec3 local_pos;

//to the first color attachment
layout(location = 0) out vec4 out_color;

layout(set = MAIN_DESC_SET_GLOBALS, binding = MAIN_BINDING_GLOBAL_GLOBAL) uniform Global
{
    GlobalData global;
};

layout(set = MAIN_DESC_SET_GLOBALS, binding = MAIN_BINDING_GLOBAL_CAMERA) uniform Global_Camera
{
    CameraData camera;
};
layout(set = MAIN_DESC_SET_GLOBALS, binding = MAIN_BINDING_GLOBAL_LIGHT) uniform Global_Lights
{
    LightData light;
};

layout(set = MAIN_DESC_SET_INSTANCES, binding = 0) readonly buffer Instance
{
    InstanceData inst[];
};

layout(set = MAIN_DESC_SET_MATERIALS, binding = 0) readonly buffer Materials
{
    MaterialData materials[];
};

layout(set = MAIN_DESC_SET_SAMPLER, binding = 0) uniform sampler  sam;

layout(set = MAIN_DESC_SET_NOISE, binding = 0) uniform texture2D tex_noise;


layout(set = MAIN_DESC_SET_STORAGE_IMAGE, binding = 0,rgba32f) uniform image2D  fragment_state;



layout(set = MAIN_DESC_SET_TEXTURES, binding = 0) uniform texture2D tex[MAX_TEXTURES];


float random(vec2 st, float time) {
    // We add time to the dot product to change the pattern every frame
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233)) + time * 1.0) * 43758.5453);
}


/*
ref->Layout.globalLayout, 
		ref->Layout.instanceLayout,
		ref->Layout.materialLayout, 
		ref->Layout.textureLayout, 
		ref->Layout.samplerLayout,
		ref->Layout.noiseTextureLayout,
		ref->Layout.storageImageLayout


		#define DESC_SET_GLOBALS 0
#define DESC_SET_INSTANCES 1
#define DESC_SET_MATERIALS 2
#define DESC_SET_TEXTURES 3
#define DESC_SET_SAMPLER 4
#define DESC_SET_NOISE 5
#define DESC_SET_STORAGE_IMAGE 6

*/

float randomRange(float time) {
    return (sin(time)  + 1.0) / 2.0;
  
}
void main() {



	// ivec2 coord = ivec2(gl_FragCoord.xy);

   	// float alpha_value = texture(sampler2D(tex_noise, sam), texCoord).r;

    vec2 jitter = texture(sampler2D(tex_noise, sam), texCoord * 10.0).rg * 2.0 - 1.0;
    vec2 sampleUV = texCoord + jitter * 1.0;

    
	// vec4 existing = imageLoad(fragment_state, coord);

	// float alpha_value = existing.x  ;

	// float direction = existing.y;

	// float speed = existing.z * 0.01;

	// alpha_value = alpha_value + direction  *  speed ;
	
	// if(alpha_value >= 1.0){
	// 	alpha_value = 1.0;
	//     direction = -1.0;  
		
	// }else if(alpha_value <= 0.0){
	// 	alpha_value = 0.0;
	//     direction = 1.0;   
	// }
	// // existing.x = alpha_value;
	// existing.y = direction;
	
	// imageStore(fragment_state, coord, existing);

   
   
    // float randVal = random(texCoord, global.time_total);
    // if (randVal > 0.1) {
    //         discard;
    // }
    
    
    InstanceData inst = inst[instance_idx];

    uint material_idx = inst.material_idx;
    if (material_idx == UNSET_VALUE) {
        material_idx = 0;
    }
    MaterialData mat = materials[material_idx];
    // mat.base_color_factor = vec4(0.0, 0.0, 1.0, 0.5);

    vec4 texColor = vec4(1.0, 0.0, 0.0, 1.0);

    if (mat.base_color_texture_idx != UNSET_VALUE)
    {
        texColor = texture(sampler2D(tex[mat.base_color_texture_idx], sam), sampleUV);

     
        
    }
    vec2 fb_size = global.framebuffer_size;

    ivec3 cam_pos = ivec3(camera.pos);
     
   	vec2 hpos = fb_size/2;

	if ( draw_value(cam_pos.x, gl_FragCoord.xy, hpos, 0.4,16) ) {
	
		texColor = vec4(2.0, 0.0, 0.0, 1.0);
	}

	hpos.y +=50;
	if ( draw_value(cam_pos.y, gl_FragCoord.xy,hpos , 0.4,16) ) {
	
		texColor = vec4(2.0, 0.0, 0.0, 1.0);
	}
	hpos.y +=50;
	if ( draw_value(cam_pos.z, gl_FragCoord.xy,hpos , 0.4,16) ) {
	
		texColor = vec4(2.0, 0.0, 0.0, 1.0);
	}

	hpos.y +=50;
	if ( draw_value(mat.base_color_factor.b  == 1.0? 1:0  , gl_FragCoord.xy,hpos , 0.4,16) ) {
	
		texColor = vec4(2.0, 0.0, 0.0, 1.0);
	}
	
	vec4 inst_color = vec4(mat.base_color_factor.xyz, 1.0);
	
	float color_factor = mat.base_color_factor.w;
	
	vec4 finalColor = mix(texColor, inst_color, color_factor);
	
	out_color = finalColor;

	// out_color.a = alpha_value;
}
