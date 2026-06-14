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



const float FADE_START = 15.0;
const float FADE_END   = 20.0; 



float gridLine(vec2 p, float size, float thickness)
{

    vec2 coord = p / size;

  
    //vec2 grid = fract(coord ) ;
     vec2 grid = abs(fract(coord - 0.5) - 0.5);


    vec2 fw = fwidth(coord);


    vec2 dist = grid / fw;


    dist /= thickness;

    float line = min(dist.x, dist.y);


    float alpha = 1.0 - smoothstep(0.0, 1.0, line);

    return alpha;
}

float computeDepth(vec3 pos)
{
    vec4 clip = camera.proj * camera.view * vec4(pos, 1.0);
    return clip.z / clip.w;
}
const vec4 COL_RED = vec4(0.8,0.2,0.2,1.0);
const vec4 COL_GREEN = vec4(0.2,0.8,0.2,1.0);
const vec4 COL_ALPHA = vec4(1.0,1.0,1.0,0.0);

const vec4 COL_GRID = vec4(0.35,0.25,0.15,0.1);



float line(vec2 uv, vec2 p1, vec2 p2, float thickness) {
    // Calculate distance from point to line segment
    vec2 dir = p2 - p1;
    vec2 toPoint = uv - p1;
    
    // Project point onto line segment
    float t = dot(toPoint, dir) / dot(dir, dir);
    t = clamp(t, 0.0, 1.0);
    
    // Find closest point on segment
    vec2 closest = p1 + t * dir;
    
    // Calculate distance
    float dist = length(uv - closest);
    
    // Return 1.0 for line, 0.0 for background
    return 1.0 - smoothstep(0.0, thickness, dist);
}



void main() {
    vec2 uv = gl_FragCoord.xy / global.framebuffer_size;  // Normalized coordinates
    
    // Example: line from (0.2, 0.8) to (0.8, 0.2)
    vec2 p1 = vec2(0.2, 0.2);
    vec2 p2 = vec2(0.8, 0.8);
    float thickness = 0.005;
    
    float lineMask = line(uv, p1, p2, thickness);
    
    vec3 bgColor = vec3(0.1, 0.1, 0.2);
    vec3 lineColor = vec3(1.0, 0.5, 0.0);
    
    outColor = vec4(mix(bgColor, lineColor, lineMask), 1.0);
}
