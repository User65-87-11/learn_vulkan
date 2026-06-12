#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require


//GPT generated


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



layout(location = 0) in vec4 fragColor;

layout(location = 0) out vec4 outColor;

void main()
{



	outColor = fragColor;


}
