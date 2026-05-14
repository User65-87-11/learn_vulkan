#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader_common.h"

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in flat uint instance_idx;

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

layout(set = DESC_SET_TEXTURES, binding = 0) uniform sampler2D tex[MAX_TEXTURES];

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
        texColor = texture(tex[mat.base_color_texture_idx], texCoord);
    }

    vec4 inst_color = vec4(mat.base_color_factor.xyz, 1.0);

    float color_factor = mat.base_color_factor.w;

    vec4 finalColor = mix(texColor, inst_color, color_factor);

    out_color = finalColor;
}
