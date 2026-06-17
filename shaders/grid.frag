


#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require


//GPT generated

#include "shader/common.h"
#include "shader/binding_grid.h"


layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_GLOBAL) uniform Global       { GlobalData global; };
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_CAMERA) uniform Global_Camera { CameraData camera; };
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_LIGHT)  uniform Global_Lights { LightData  light;  };


layout(location = 0) in  vec3 nearPoint;
layout(location = 1) in  vec3 farPoint;
layout(location = 0) out vec4 outColor;

const float FADE_START = 5.0;
const float FADE_END   = 12.0; 

const float FADE_END_2   = 20.0; 

const vec3 COL_RED = vec3(0.8, 0.2, 0.2);
const vec3 COL_GRID_1 = vec3(0.04);
const vec3 COL_GRID_10 = vec3(0.08);
const vec3 COL_GREEN = vec3(0.0,1.0,0.0);
const vec3 COL_BLUE = vec3(0.2, 0.6, 1.0);
const vec3 COL_BG = vec3(0.01f);


float gridLine(vec2 p, float s, float thickness)
{

    vec2 coord = p / s;

    vec2 distGrid = abs(fract(coord - 0.5) - 0.5);

    vec2 fw = fwidth(coord);

    vec2 distFrag = distGrid / fw;

    float line = min(distFrag.x, distFrag.y);

    
    
    float alpha = 1.0 - smoothstep( 0, thickness , line);

    return alpha;
}

float computeDepth(vec3 pos)
{
    vec4 clip = camera.view_proj * vec4(pos, 1.0);
    return clip.z / clip.w;
}

float axisLineX(vec2 p, float s)
{
    vec2 coord = p / s;
    vec2 fw = fwidth(coord);
    // this has to be 0
    float d = abs(coord.x) / fw.x;
    return 1.0 - smoothstep(0.0, 1.0, d);
}

float axisLineZ(vec2 p, float s)
{
    vec2 coord = p / s;
    vec2 fw = fwidth(coord);
    float d = abs(coord.y) / fw.y;
    return 1.0 - smoothstep(0.0, 1.0, d);
}
bool equals_zero(float v){
	if(abs(v) < 0.001){
		return true;
	}
	return false;
}
void main()
{
    float denom = (farPoint.y - nearPoint.y);
    


    
    
    float t = -nearPoint.y / denom;
    if (t <= 0.0 || t > 1.0) discard;

    vec3 point  = nearPoint + t * (farPoint - nearPoint);
    vec3 camPos = camera.pos.xyz;


    float dist = distance(camPos, point);

    float distanceFade = 1.0 - smoothstep(FADE_START, FADE_END, dist);
     float distanceFade2 =1.0 - smoothstep(FADE_START, FADE_END_2, dist);

   
    vec3 cappedPoint = camPos + normalize(point - camPos) * min(dist, FADE_END_2);

    gl_FragDepth = computeDepth(cappedPoint);

     
    float g1 = gridLine(point.xz, 0.5, 1);
    float g10 = gridLine(point.xz, 5, 1);

  
    
    float axisX = axisLineX(point.xz, 0.5); 
    float axisZ = axisLineZ(point.xz, 0.5); 


    vec3 color = COL_GRID_1;
    
    color = mix(color,COL_GRID_10,g10);
    color = mix(color, COL_RED, axisX);
    color = mix(color, COL_BLUE, axisZ);

    
    if(equals_zero(g10) == false)
    {
    
    	outColor = vec4(color, min(g10,distanceFade2));
    }else
    {
    	outColor = vec4(color, min(g1,distanceFade));
    }
    
    
  

   
}
