#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

layout(binding=1) uniform sampler2D  tex;

void main() {
	vec4 texColor = mix(texture(tex, texCoord), fragColor, 0.1);

    outColor = texColor;
}
