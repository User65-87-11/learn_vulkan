#pragma once
#include <vulkan/vulkan_core.h>
#include "util/common.h"
#include "cglm/types.h"



typedef struct Mesh {
    uint32_t vertex_offset;
    uint32_t index_offset;

    uint32_t vertex_count;
    uint32_t index_count;
} Mesh;



struct Camera{
	vec3 pos;
	vec3 front;
	vec3 up ;

	float yaw;
	float pitch ;
	float lastX ;
	float lastY;
	float fov ;
};


struct Camera * Scene_GetCamera(); 

void Scene_Init();
void Scene_Update(float dt);