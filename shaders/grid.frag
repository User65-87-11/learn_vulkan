


#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require


//GPT generated

#include "shader/common.h"
#include "shader/binding_grid.h"

layout(location = 0) in  vec3 nearPoint;
layout(location = 1) in  vec3 farPoint;
layout(location = 0) out vec4 outColor;

layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_GLOBAL) uniform Global       { GlobalData global; };
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_CAMERA) uniform Global_Camera { CameraData camera; };
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_LIGHT)  uniform Global_Lights { LightData  light;  };



const float FADE_START = 5.0;
const float FADE_END   = 10.0; 



float gridLine(vec2 p, float s, float thickness)
{

    vec2 coord = p / s;

    //instead of 1-800 > 400/1/400/800
     vec2 grid = abs(fract(coord - 0.5) - 0.5);



    
     //(abs(dFdx(gl_FragCoord.x))   < 1.001)
     // (abs(dFdx(coord.x))+abs(dFdy(coord.y)))
    vec2 fw = fwidth(coord);


    vec2 dist = grid / fw;


    dist /= thickness;

    float line = min(dist.x, dist.y);


    float alpha = 1.0 - smoothstep(0.0, thickness, line);

    return alpha;
}

float computeDepth(vec3 pos)
{
    vec4 clip = camera.proj * camera.view * vec4(pos, 1.0);
    return clip.z / clip.w;
}
const vec4 COL_RED = vec4(1.0,0.0,0.0,1.0);
const vec4 COL_GRID_1 = vec4(0.1,0.1,0.1,1.0);
const vec4 COL_GRID_10 = vec4(0.1,0.1,0.1,1.0);
const vec4 COL_GREEN = vec4(0.0,1.0,0.0,1.0);
const vec4 COL_BG = vec4(0.01f, 0.01f, 0.01f, 1.0f);
/*

vec3 gridPlane[3] = vec3[](
    vec3(-1.0, -1.0, 0.0),
    vec3( 3.0, -1.0, 0.0),
    vec3(-1.0,  3.0, 0.0)
);

*/


void main()
{
    float denom = (farPoint.y - nearPoint.y);
    outColor = COL_GRID_1;


    
    
    float t = -nearPoint.y / denom;
    if (t <= 0.0 || t > 1.0) discard;

    vec3 point  = nearPoint + t * (farPoint - nearPoint);
    vec3 camPos = camera.pos.xyz;


    float dist = distance(camPos, point);

    float distanceFade = 1.0 - smoothstep(FADE_START, FADE_END, dist);

   
    vec3 cappedPoint = camPos + normalize(point - camPos) * min(dist, FADE_END);
 
    float g1 = gridLine(point.xz, 0.5, 1);
    
    outColor = vec4(outColor.xyz, g1 * distanceFade);

    gl_FragDepth = computeDepth(cappedPoint);
}
