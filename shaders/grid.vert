#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader/common.h"
#include "shader/binding_grid.h"




layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_GLOBAL) uniform Global
{
    GlobalData global;
};

layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_CAMERA) uniform Global_Camera
{
    CameraData camera;
};
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_LIGHT) uniform Global_Lights
{
    LightData light;
};


vec2 positions[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
);

layout(location = 0) out vec3 nearPoint;
layout(location = 1) out vec3 farPoint;

vec3 unprojectPoint(float x, float y, float z) {
    mat4 viewInv = camera.inv_view;
    mat4 projInv = camera.inv_proj;
    vec4 unprojected = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojected.xyz / unprojected.w;
}

void main() {
    vec2 p = positions[gl_VertexIndex];
    // Unproject near/far plane points into world space
    nearPoint = unprojectPoint(p.x, p.y, -1.0); // near plane in GL NDC
    farPoint  = unprojectPoint(p.x, p.y,  1.0); // far plane
   gl_Position = vec4(p, 0.0, 1.0);

   
}
