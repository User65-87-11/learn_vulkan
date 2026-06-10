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

float gridLine(vec2 uv, float scale) {
    vec2 coord  = uv * scale;
    vec2 deriv  = fwidth(coord);
    vec2 grid   = abs(fract(coord - 0.5) - 0.5) / max(deriv, vec2(0.0001));
    return 1.0 - min(min(grid.x, grid.y), 1.0);
}

void main() {
	float denom = farPoint.y - nearPoint.y;
    if (abs(denom) < 1e-6) discard;

    float t = -nearPoint.y / denom;
    if (t < 0.0) discard;

    outColor = vec4(t * 0.1, 0.0, 0.0, 1.0);
}
