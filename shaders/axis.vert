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

layout(location = 0) out vec4 outColor;


const vec3 verts[18] =
{
    // X axis
    {-1.0f, -1.0f,  0.0f},
    { 1.0f, -1.0f,  0.0f},
    { 1.0f,  1.0f,  0.0f},

    {-1.0f, -1.0f,  0.0f},
    { 1.0f,  1.0f,  0.0f},
    {-1.0f,  1.0f,  0.0f},

    // Y axis
    {-1.0f, -1.0f,  0.0f},
    { 1.0f, -1.0f,  0.0f},
    { 1.0f,  1.0f,  0.0f},

    {-1.0f, -1.0f,  0.0f},
    { 1.0f,  1.0f,  0.0f},
    {-1.0f,  1.0f,  0.0f},

    // Z axis
    {-1.0f,  0.0f, -1.0f},
    { 1.0f,  0.0f, -1.0f},
    { 1.0f,  0.0f,  1.0f},

    {-1.0f,  0.0f, -1.0f},
    { 1.0f,  0.0f,  1.0f},
    {-1.0f,  0.0f,  1.0f},
};

const vec3 COL_YELLOW = vec3(1.0,1.0,0.0);

void main()
{





	uint axis = gl_VertexIndex / 6;
	vec3 vert = verts[gl_VertexIndex];
	vec3 point = vec3(0.0);
	vec3 color = vec3(1.0);
	float len = 5;
	float thich = 100;
	switch(axis){
		case 0:
			point = vec3(vert.x * len, vert.y/thich, 	vert.z);
			color = vec3(1.0,0.0,0.0);
		break;
		case 1:
			point = vec3(vert.x /thich, vert.y*len,	vert.z);
			color = vec3(0.0,1.0,0.0);
		break;
		case 2:
			point = vec3(vert.x / thich, vert.y, 	vert.z*len);
			color = vec3(0.0,0.0,1.0);
		break;
	}

	

	//--CHANGE ONLY THIS PART STARTS

	vec3 camPos = camera.pos;

	

    gl_Position = camera.proj * camera.view * vec4(point, 1.0);

    if(gl_Position.z < 0)
    	color = COL_YELLOW;
    //--CHANGE ONLY THIS PART ENDS
    
    outColor = vec4(color, 1.0);


    /*
    TODO:
    	put points relative to camera position?

    */
}
