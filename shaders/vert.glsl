#version 450
#extension GL_GOOGLE_include_directive : require

#include "shader_inc.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_texCoord;

layout(set = DESC_SET_GLOBALS, binding = 0) uniform Global
{
    Global_ubo global;
};

layout(std430, set = DESC_SET_INSTANCES, binding = 0) readonly buffer Instance
{
    Instance_ssbo inst[];
};

layout(std430, set = DESC_SET_MATERIALS, binding = 0) readonly buffer Materials
{
    Material_ssbo material[];
};

layout(location = 0) out vec2 out_texCoord;

layout(location = 1) out vec3 out_norm;

layout(location = 2) out flat uint out_instance_id;

void main() {
    vec3 disstorted_pos = in_position;

    // disstorted_pos.x += (sin(in_position.y * 10.0 + global.time_total * 5) + 1.0) * 0.05;

    vec4 world_pos = inst[gl_InstanceIndex].model * vec4(disstorted_pos, 1.0);

    gl_Position = global.proj * global.view * world_pos;

    out_texCoord = in_texCoord;

    out_norm = in_norm;

    out_instance_id = gl_InstanceIndex;
}
