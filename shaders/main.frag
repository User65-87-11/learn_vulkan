#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "debug.glsl"
#include "shader_common.h"

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in flat uint instance_idx;

layout(location = 3) in vec3 local_pos;

//to the first color attachment
layout(location = 0) out vec4 out_color;

layout(set = DESC_SET_GLOBALS, binding = BINDING_GLOBAL_GLOBAL) uniform Global
{
    GlobalData global;
};

layout(set = DESC_SET_GLOBALS, binding = BINDING_GLOBAL_CAMERA) uniform Global_Camera
{
    CameraData camera;
};
layout(set = DESC_SET_GLOBALS, binding = BINDING_GLOBAL_LIGHT) uniform Global_Lights
{
    LightData light;
};

layout(set = DESC_SET_INSTANCES, binding = 0) readonly buffer Instance
{
    InstanceData inst[];
};

layout(set = DESC_SET_MATERIALS, binding = 0) readonly buffer Materials
{
    MaterialData materials[];
};

layout(set = DESC_SET_SAMPLER, binding = 0) uniform sampler  sam;


layout(set = DESC_SET_TEXTURES, binding = 0) uniform texture2D tex[MAX_TEXTURES];

layout(set = DESC_SET_NOISE, binding = 0) uniform texture2D tex_noise;

/*

*/
float random(vec2 st, float time) {
    // We add time to the dot product to change the pattern every frame
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233)) + time * 1.0) * 43758.5453);
}

void main() {

    // if (idx == 0 && inst[idx].tex_idx == 0)
    // {
    //     out_color = vec4(1.0, 0.0, 0.0, 1.0);
    // } else
    // {
    //     out_color = vec4(0.2, 0.8, 1.0, 1.0);
    // }
    // return;

    // uint idx = instance_id;

    // uint material_idx = inst[idx].material_id;

    // vec2 uv = local_pos.xy; 


    
    // float noise_value = texture(sampler2D(tex_noise, sam), uv).r;

    // if (noise_value > 0.5) {
    //         discard;
    // }

   // float slowTime = floor(global.time_total); 

    float randVal = random(texCoord, 1);
        
   // float treshold = texture(sampler2D(tex_noise, sam), texCoord + vec2(global.time_total * 0.5)).r;

    if (randVal > 0.1) {
            discard;
    }
    
    
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
        texColor = texture(sampler2D(tex[mat.base_color_texture_idx], sam), texCoord);

         if (texColor.r > 0.5) {
                 discard;
         }
        
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
	
	vec4 inst_color = vec4(mat.base_color_factor.xyz, 1.0);
	
	float color_factor = mat.base_color_factor.w;
	
	vec4 finalColor = mix(texColor, inst_color, color_factor);
	
	out_color = finalColor;
}
