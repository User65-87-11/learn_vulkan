#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require


//GPT generated
#include "debug.glsl"
#include "shader/common.h"
#include "shader/binding_grid.h"

layout(location = 0) in  vec3 nearPoint;
layout(location = 1) in  vec3 farPoint;
layout(location = 0) out vec4 outColor;

layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_GLOBAL) uniform Global       { GlobalData global; };
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_CAMERA) uniform Global_Camera { CameraData camera; };
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_LIGHT)  uniform Global_Lights { LightData  light;  };

const float AXIS_GLOW_WIDTH = 2.0;

const float GRID_SCALE_1  = 1.0;
const float GRID_SCALE_10 = 10.0;

const vec4 COLOR_GRID = vec4(0.25, 0.25, 0.25, 0.5);

float gridLine(vec2 p, float s, float thickness)
{
    // work in world space first
    vec2 coord = p / s;

    // distance to nearest grid line in cell space
    vec2 grid = abs(fract(coord - 0.5) - 0.5);

    // screen-space AA scale
    vec2 fw = fwidth(coord);

    // normalize distance into pixel space
    vec2 dist = grid / fw;

    // apply controllable thickness (in pixels)
    dist /= thickness;

    float line = min(dist.x, dist.y);

    // smooth falloff instead of hard clamp
    float alpha = 1.0 - smoothstep(0.0, 1.0, line);

    return alpha;
}

float computeDepth(vec3 pos)
{
    vec4 clip = camera.proj * camera.view * vec4(pos, 1.0);
    return clip.z / clip.w;
}
void main()
{
    // -------------------------------
    // 1. RAY-PLANE INTERSECTION
    // line equation 
    // x(t) = x0 + tx1
    // P(t) = P0 + t * (P1 - P0)
    // LERP
    // t = (P - P0) / (P1 - P0)
    // t = Interpolation factor 
    // P(t) = 0 , reaches 0
    // -------------------------------
    float denom = (farPoint.y - nearPoint.y);
    
    if (abs(denom) < 0.001) discard;

    // t = (0 - P0) / (P1 - P0) the angle of the line (Interpolation factor )
    float t = -nearPoint.y / denom;
    if (t <= 0.0 || t > 1.0) discard;

    // interpolation
    vec3 pos = nearPoint + t * (farPoint - nearPoint);

    // -------------------------------
    // 2. DEPTH CALCULATION
    // -------------------------------
    float depth = computeDepth(pos);
    if (depth < 0.0 || depth > 1.0) discard;

    gl_FragDepth = depth;

    // -------------------------------
        // 3. GRID GENERATION
        // -------------------------------
        float g1  = gridLine(pos.xz, GRID_SCALE_1, 1);
        float g10 = gridLine(pos.xz, GRID_SCALE_10, 1);
    
        float gridMask = max(g1, g10);
        
        // FIX: Use 1.0 for the line intensity to get pure white
        vec3 grid = vec3(1.0) *gridMask; 

    // -------------------------------
    // 4. FADING LOGIC
    // -------------------------------
    vec3 camPos = camera.pos.xyz;
    float dist = distance(camPos, pos);
    
    // Horizon fade: makes the grid disappear as it gets flat against the screen
    float horizon = abs(dot(normalize(pos - camPos), vec3(0.0, 1.0, 0.0)));
    float horizonFade = pow(horizon, 0.8);

    // Distance fade: makes the grid disappear as it gets far away
    float distanceFade = 1.0; // Default to fully visible
    
    // Check if there is actually a range to fade over
    if (abs(FADE_END - FADE_START) > 0.001) {
        // There is a difference: Apply smooth fade
        distanceFade = 1.0 - smoothstep(FADE_START, FADE_END, dist);
    } 
    
    float fade = horizonFade * distanceFade;

    // -------------------------------
    // 5. FINAL OUTPUT
    // -------------------------------
    vec3 finalColor = vec3(1.0);
    float finalAlpha = gridMask * fade;

    outColor = vec4(finalColor, finalAlpha);

    if (outColor.a < 0.001) discard;
}
