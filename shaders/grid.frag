#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#include "shader/common.h"
#include "shader/binding_grid.h"

layout(location = 0) in  vec3 nearPoint;
layout(location = 1) in  vec3 farPoint;
layout(location = 0) out vec4 outColor;

layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_GLOBAL) uniform Global       { GlobalData global; };
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_CAMERA) uniform Global_Camera { CameraData camera; };
layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_LIGHT)  uniform Global_Lights { LightData  light;  };

const float FADE_START = 60.0;
const float FADE_END   = 300.0;
const float FADE_POWER  = 2.5;



const float GRID_SCALE_1  = 1.0;
const float GRID_SCALE_10 = 10.0;

const vec4 COLOR_GRID  = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 COLOR_XAXIS = vec4(0.90, 0.25, 0.25, 1.0);
const vec4 COLOR_ZAXIS = vec4(0.25, 0.55, 0.90, 1.0);

float gridLine(vec2 p, float s) {
    vec2 coord = p / s;
    vec2 grid  = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid.x, grid.y);
    return 1.0 - min(line, 1.0);
}

float computeDepth(vec3 pos) {
    vec4 clip = camera.proj * camera.view * vec4(pos, 1.0);
    return clip.z / clip.w;
}

void main() {


    // -------------------------------
    // ray-plane intersection (y = 0)
    // -------------------------------
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    if (t <= 0.0 || t > 1.0) discard;

    vec3 pos = nearPoint + t * (farPoint - nearPoint);

    // optional depth reject (kept, but not used for fading)
    float depth = computeDepth(pos);
    if (depth < 0.0 || depth > 1.0) discard;

    gl_FragDepth = depth;
    outColor = vec4(1.0,0.0,0.0,1.0);
	
		return;
    // -------------------------------
    // GRID LINES
    // -------------------------------
    float g1  = gridLine(pos.xz, GRID_SCALE_1);
    float g10 = gridLine(pos.xz, GRID_SCALE_10);

    vec4 gridColor = mix(COLOR_GRID * 0.7, COLOR_GRID, max(g1, g10 * 1.3));

    // -------------------------------
    // FADE (FIXED)
    // -------------------------------

    vec3 camPos = camera.pos.xyz;

    float dist = distance(camPos, pos);

    vec3 viewDir = normalize(pos - camPos);

    float horizonFade =
        pow(abs(dot(viewDir, vec3(0.0, 1.0, 0.0))), 1.0);


    float fade =  horizonFade;


    // -------------------------------
    // AXIS HIGHLIGHT
    // -------------------------------
    float xAxis = max(0.0, 1.0 - abs(pos.z) / max(fwidth(pos.z) * 2.0, 0.008));
    float zAxis = max(0.0, 1.0 - abs(pos.x) / max(fwidth(pos.x) * 2.0, 0.008));

    vec4 color = gridColor;

    color = mix(color, COLOR_XAXIS, xAxis * fade);
    color = mix(color, COLOR_ZAXIS, zAxis * fade);

    float alpha = max(g1, g10) * fade;

    outColor = vec4(color.rgb, alpha);

    if (outColor.a < 0.001) discard;
}
