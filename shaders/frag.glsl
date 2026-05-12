#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_common.h"

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in flat uint instance_id;

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
    MaterialData material[];
};

layout(set = DESC_SET_TEXTURES, binding = 0) uniform sampler2D tex[MAX_TEXTURES];

void main() {
    if (global.frame_cnt == 0x88)
    {
        out_color = vec4(1.0, 0.0, 0.0, 1.0);
    } else
    {
        out_color = vec4(0.2, 0.8, 1.0, 1.0);
    }
    return;
    uint idx = instance_id;

    uint tex_idx = inst[idx].tex_idx;

    vec2 distorted_uv = texCoord;
    // distorted_uv.x += sin(texCoord.y * 10.0 + global.time_total) * 0.05;

    vec4 texColor = texture(tex[tex_idx], distorted_uv);

    vec4 inst_color = inst[idx].color;

    float color_factor = inst[idx].color_factor;

    vec4 finalColor = mix(texColor, inst_color, color_factor);

    out_color = finalColor;
}
