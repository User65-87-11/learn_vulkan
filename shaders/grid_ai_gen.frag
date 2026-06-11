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
    // ray-plane intersection
    // -------------------------------
    float denom = (farPoint.y - nearPoint.y);
    
    if (abs(denom) < 0.001) discard;

    float t = -nearPoint.y / denom;
    if (t <= 0.0 || t > 1.0) discard;

    vec3 pos = nearPoint + t * (farPoint - nearPoint);

    // -------------------------------
    // depth
    // -------------------------------
    float depth = computeDepth(pos);
    if (depth < 0.0 || depth > 1.0) discard;

    gl_FragDepth = depth;

    // -------------------------------
    // GRID (screen-space)
    // -------------------------------
    float g1  = gridLine(pos.xz, GRID_SCALE_1,1);
    float g10 = gridLine(pos.xz, GRID_SCALE_10,1);

    float gridMask = max(g1, g10);
    
    vec3 grid = COLOR_GRID.rgb * (0.20 + 0.60 * gridMask);

    
    // -------------------------------
    // FADE
    // -------------------------------
    vec3 camPos = camera.pos.xyz;

    float dist = distance(camPos, pos);

    float horizon = abs(dot(normalize(pos - camPos), vec3(0.0, 1.0, 0.0)));
    float horizonFade = pow(horizon, 1.2);

    float distanceFade = 1.0 / (1.0 + dist * dist * 0.002);

    float fade = horizonFade * distanceFade;

    // -------------------------------
    // AXIS (FIXED: screen-space width)
    // -------------------------------
    float axisW = AXIS_GLOW_WIDTH;

    float xAxis = 1.0 - min(abs(pos.z) / fwidth(pos.z) / axisW, 1.0);
    float zAxis = 1.0 - min(abs(pos.x) / fwidth(pos.x) / axisW, 1.0);

    float xCore = xAxis * xAxis;
    float zCore = zAxis * zAxis;

    float xGlow = xCore * 0.5;
    float zGlow = zCore * 0.5;

    vec3 xCol = vec3(1.8, 0.25, 0.25);
    vec3 zCol = vec3(0.25, 0.55, 1.8);

    vec3 axis = vec3(0.0);
    axis += xCol * (xCore + xGlow) * 4.0;
    axis += zCol * (zCore + zGlow) * 4.0;

    // -------------------------------
    // COMBINE
    // -------------------------------
    vec3 color = grid * fade;
    color += axis * fade;

    float alpha = gridMask * fade;

    outColor = vec4(color, alpha);

    if (outColor.a < 0.001) discard;
}
