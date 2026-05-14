#version 450
#extension GL_GOOGLE_include_directive : require

#include "shader_common.h"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_texCoord;

layout(set = DESC_SET_GLOBALS, binding = BINDING_GLOBAL_GLOBAL) uniform Global
{
    GlobalData global;
};

layout(set = DESC_SET_GLOBALS, binding = BINDING_GLOBAL_CAMERA) uniform Global_Camera
{
    CameraData camera[2];
};
layout(set = DESC_SET_GLOBALS, binding = BINDING_GLOBAL_LIGHT) uniform Global_Lights
{
    LightData light;
};

layout(set = DESC_SET_INSTANCES, binding = 0) readonly buffer Instance
{
    InstanceData instance[];
};

layout(set = DESC_SET_MATERIALS, binding = 0) readonly buffer Materials
{
    MaterialData material[];
};

layout(location = 0) out vec2 out_texCoord;

layout(location = 1) out vec3 out_norm;

layout(location = 2) out flat uint out_instance_idx;

mat4 get_proj() {
    float fov = radians(45.0);
    float aspect = 16.0 / 9.0;
    float near = 0.1;
    float far = 10.0;

    float f = 1.0 / tan(fov * 0.5);

    mat4 proj = mat4(
            f / aspect, 0.0, 0.0, 0.0,
            0.0, f, 0.0, 0.0,
            0.0, 0.0, (far + near) / (near - far), -1.0,
            0.0, 0.0, (2.0 * far * near) / (near - far), 0.0
        );

    proj[1][1] *= -1.0;
    return proj;
}
vec3 positions[3] = vec3[](
        vec3(-1.6, -1.4, -6.0),
        vec3(1.6, -1.4, -6.0),
        vec3(1.0, 1.6, -6.0)
    );

void main() {
    // mat4 view = mat4(1.0);

    // Hard-coded triangle vertices

    // Select vertex based on built-in ID
    // vec3 pos = positions[gl_VertexIndex];

    // gl_Position = vec4(pos, 0.0, 1.0);
    // gl_Position = camera.view_proj * vec4(in_position, 1.0);
    // return;

    InstanceData inst = instance[gl_InstanceIndex];
    vec4 world_pos = inst.model * vec4(in_position, 1.0);

 
    gl_Position = camera[inst.camera_idx].view_proj * world_pos;
  

    out_texCoord = in_texCoord;

    out_norm = in_norm;

    out_instance_idx = gl_InstanceIndex;
}
