#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "shader/common.h"
#include "shader/binding_hud.h"

layout(location = 0) in vec2 in_texCoord;

layout(location = 1) in vec4 in_fragColor;



layout(location = 0) out vec4 outColor;



layout(set = HUD_DESC_SET_GLOBALS, binding = HUD_BINDING_GLOBAL_GLOBAL) uniform Global
{
    GlobalData global;
};

layout(set = HUD_DESC_SET_GLOBALS, binding = HUD_BINDING_GLOBAL_CAMERA) uniform Global_Camera
{
    Camera2DData camera;
};

layout(set = HUD_DESC_SET_INSTANCES, binding = 0) readonly buffer Instance
{
    InstanceData instance[];
};

layout(set = HUD_DESC_SET_MATERIALS, binding = 0) readonly buffer Materials
{
    MaterialData material[];
};




layout(set = HUD_DESC_SET_TEXTURES, binding = 0) uniform texture2D tex[MAX_HUD_TEXTURES];

layout(set = HUD_DESC_SET_SAMPLER, binding = 0) uniform sampler2D  sam;




void main() {
	// // vec4 texColor = mix(texture(tex, texCoord), fragColor, 0.1);

	


    outColor = in_fragColor;

	// outColor = vec4(1.0,1.0,1.0,1.0);

}
