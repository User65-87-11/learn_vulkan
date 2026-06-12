#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader/common.h"
#include "shader/binding_axis.h"




layout(set = AXIS_DESC_SET_GLOBALS, binding = AXIS_BINDING_GLOBAL_GLOBAL) uniform Global
{
    GlobalData global;
};

layout(set = AXIS_DESC_SET_GLOBALS, binding = AXIS_BINDING_GLOBAL_CAMERA) uniform Global_Camera
{
    CameraData camera;
};
layout(set = AXIS_DESC_SET_GLOBALS, binding = AXIS_BINDING_GLOBAL_LIGHT) uniform Global_Lights
{
    LightData light;
};

layout(location = 0) out vec4 fragColor;



void main()
{
    int id = gl_VertexIndex;

    float A = camera.proj[2][2];
    float B = camera.proj[3][2];
    float far = B / (A - 1.0);
    
    float L = far * 0.99; // nearly full far plane distance

    vec3 pos = vec3(0.0);
    vec4 col = vec4(1.0);

   float  fragAlpha = 0.0;
    // X Axis (Red)
    if      (id == 0) { pos = vec3(-L,  0.0, 0.0); col = vec4(1.0, 0.2, 0.2,fragAlpha);}
    else if (id == 1) { pos = vec3( 0.0, 0.0, 0.0); col = vec4(1.0, 0.2, 0.2,fragAlpha); }
    else if (id == 2) { pos = vec3( 0.0, 0.0, 0.0); col = vec4(1.0, 0.2, 0.2,fragAlpha); }
    else if (id == 3) { pos = vec3( L,  0.0, 0.0);fragAlpha = 1.0; col = vec4(1.0, 0.0, 0.0,fragAlpha);  }

    // Y Axis (Green)
    else if (id == 4) { pos = vec3(0.0, -L/50,  0.0); col = vec4(0.2, 0.2, 0.2,fragAlpha); }
    else if (id == 5) { pos = vec3(0.0,  0.0, 0.0); col = vec4(0.2, 0.2, 0.2,fragAlpha); }
    else if (id == 6) { pos = vec3(0.0,  0.0, 0.0); col = vec4(0.2, 0.2, 0.2,fragAlpha); }
    else if (id == 7) { pos = vec3(0.0,  L/50,  0.0);fragAlpha = 1.0;col = vec4(0.0, 1.0, 0.0,fragAlpha); }

    // Z Axis (Blue)
    else if (id == 8)  { pos = vec3(0.0, 0.0, -L);  col = vec4(0.2, 0.2, 1.0,fragAlpha); }
    else if (id == 9)  { pos = vec3(0.0, 0.0,  0.0); col = vec4(0.2, 0.2, 1.0,fragAlpha); }
    else if (id == 10) { pos = vec3(0.0, 0.0,  0.0); col = vec4(0.2, 0.2, 1.0,fragAlpha); }
    else if (id == 11) { pos = vec3(0.0, 0.0,  L); fragAlpha = 1.0;  col = vec4(0.0, 0.0, 1.0,fragAlpha); }



    
   

        
    gl_Position = camera.proj * camera.view * vec4(pos, 1.0);
    fragColor = col;
}
