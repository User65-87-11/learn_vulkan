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

layout(location = 0) out vec3 nearPoint;
layout(location = 1) out vec3 farPoint;


vec3 gridPlane[3] = vec3[](
    vec3(-1.0, -1.0, 0.0),
    vec3( 3.0, -1.0, 0.0),
    vec3(-1.0,  3.0, 0.0)
);

vec3 unprojectPoint(float x, float y, float z) {

    vec4 unprojected = camera.inv_view * camera.inv_proj * vec4(x, y, z, 1.0);
    return unprojected.xyz / unprojected.w;
}

void main() {
	vec3 p = gridPlane[gl_VertexIndex];
    // Z=0.0 is near plane, Z=1.0 is far plane in Vulkan [0,1]
    nearPoint  = unprojectPoint(p.x, p.y, 0.0);
    farPoint   = unprojectPoint(p.x, p.y, 1.0);

    
   gl_Position = vec4(p, 1.0);

   
}
