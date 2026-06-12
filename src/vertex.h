#pragma once
#include "cglm/types.h"

struct Vertex {
	vec3 pos;
	vec3 norm;
	vec2 texCoords;
};

struct Vertex2D{
	vec2 pos;
	vec2 texCoords;
	vec4 color;
};


struct VertexColor{
	vec3 pos;
	vec4 color;
};
