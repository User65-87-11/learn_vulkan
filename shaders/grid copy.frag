#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require


//GPT generated

#include "shader/common.h"
#include "shader/binding_grid.h"



layout(location = 0) in vec3 fragWorldPos;
layout(location = 1) in vec3 fragViewDir;
layout(location = 2) in float fragDistance;

layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_GLOBAL) uniform Global
{
    GlobalData global;
};

layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_CAMERA) uniform Global_Camera
{
    CameraData camera;
};

layout(set = GRID_DESC_SET_GLOBALS, binding = GRID_BINDING_GLOBAL_LIGHT) uniform Global_Lights
{
    LightData light;
};

layout(location = 0) out vec4 outColor;

// Grid parameters
const float gridSize = 1.0;
const float gridThickness = 0.02;
const float majorGridSize = 10.0;
const float majorGridThickness = 0.04;
const float gridFadeDistance = 50.0;
const float gridMaxDistance = 100.0;

// Anti-aliased grid line calculation
float computeGridLine(vec2 pos, float size, float thickness)
{
    vec2 coord = pos / size;
    
    // fwidth gives the screen-space derivative, perfect for anti-aliasing
    vec2 derivative = fwidth(coord);
    
    // Distance to the nearest grid line (ranges from 0.0 at the line to 0.5 between lines)
    vec2 grid = abs(fract(coord - 0.5) - 0.5);
    
    // Smoothstep creates a smooth transition over 1 pixel to prevent jagged edges
    vec2 line = 1.0 - smoothstep(thickness - derivative, thickness + derivative, grid);
    
    return max(line.x, line.y);
}

void main()
{
    // Early discard for invalid intersections or points behind the camera
    if (fragDistance <= 0.0 || fragDistance > gridMaxDistance) {
        discard;
    }
    
    // Calculate distance-based fade
    float fadeFactor = 1.0 - smoothstep(gridFadeDistance, gridMaxDistance, fragDistance);
    
    // Grid position (XZ plane)
    vec2 gridPos = fragWorldPos.xz;
    
    // Compute regular and major grids
    float grid = computeGridLine(gridPos, gridSize, gridThickness);
    float majorGrid = computeGridLine(gridPos, majorGridSize, majorGridThickness);
    
    // Blend grids (major grid cleanly overrides minor grid)
    float finalGrid = max(grid * 0.6, majorGrid);
    finalGrid *= fadeFactor;
    
    // Discard if too transparent to save fill rate
    if (finalGrid < 0.01) {
        discard;
    }
    
    // Grid colors
    vec3 minorColor = vec3(0.5, 0.5, 0.5);
    vec3 majorColor = vec3(0.8, 0.8, 0.8);
    vec3 gridColor = mix(minorColor, majorColor, majorGrid);
    
    // View angle shading (makes it look nicer at grazing angles)
    vec3 normal = vec3(0.0, 1.0, 0.0);
    float viewDot = abs(dot(normalize(-fragViewDir), normal));
    gridColor *= (0.7 + 0.3 * viewDot);
    
    outColor = vec4(gridColor, finalGrid * 0.8);
}
