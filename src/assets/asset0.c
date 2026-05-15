#include <stdint.h>
#include <string.h>
#include "../descriptor.h"
#include "../renderer.h"
#include "../scene.h"
#include "../shader_common.h"
#include "../util/common.h"
#include "../vertex.h"
#include "../resource.h"
#include "asset0.h"

static uint8_t  image_png0[] = 
{
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7A, 0x7A, 0xF4, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x01, 0xD9, 0xC9, 0x2C, 0x7F, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xB1, 0x8F, 0x0B, 0xFC, 0x61, 0x05, 0x00, 0x00, 0x00, 0x20, 0x63, 0x48, 0x52, 0x4D, 0x00, 0x00, 0x7A, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00, 0xFA, 0x00, 0x00, 0x00, 0x80, 0xE8, 0x00, 0x00, 0x75, 0x30, 0x00, 0x00, 0xEA, 0x60, 0x00, 0x00, 0x3A, 0x98, 0x00, 0x00, 0x17, 0x70, 0x9C, 0xBA, 0x51, 0x3C, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x2E, 0x23, 0x00, 0x00, 0x2E, 0x23, 0x01, 0x78, 0xA5, 0x3F, 0x76, 0x00, 0x00, 0x00, 0x41, 0x49, 0x44, 0x41, 0x54, 0x58, 0xC3, 0xED, 0xD6, 0xC1, 0x09, 0x00, 0x30, 0x0C, 0x42, 0xD1, 0xA4, 0x3B, 0x9A, 0x21, 0x5D, 0xD2, 0x0E, 0x91, 0x43, 0x29, 0x7C, 0xEF, 0xC2, 0x03, 0x2F, 0xB6, 0xA4, 0xD4, 0x22, 0xB6, 0x37, 0xF5, 0x3A, 0xF5, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x4E, 0xB2, 0xFA, 0x03, 0x33, 0xC3, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xDF, 0x80, 0x0B, 0xBB, 0xA7, 0x09, 0x39, 0x81, 0xAB, 0x3D, 0x73, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

// static struct Vertex cube0_vertices[] = {
//     // +Z (front)
//     {{-0.5f,-0.5f, 0.5f}, {0,0,1}, {0,0}},
//     {{ 0.5f,-0.5f, 0.5f}, {0,0,1}, {1,0}},
//     {{ 0.5f, 0.5f, 0.5f}, {0,0,1}, {1,1}},
//     {{-0.5f, 0.5f, 0.5f}, {0,0,1}, {0,1}},
//     // -Z (back)
//     {{ 0.5f,-0.5f,-0.5f}, {0,0,-1}, {0,0}},
//     {{-0.5f,-0.5f,-0.5f}, {0,0,-1}, {1,0}},
//     {{-0.5f, 0.5f,-0.5f}, {0,0,-1}, {1,1}},
//     {{ 0.5f, 0.5f,-0.5f}, {0,0,-1}, {0,1}},
//     // -X (left)
//     {{-0.5f,-0.5f,-0.5f}, {-1,0,0}, {0,0}},
//     {{-0.5f,-0.5f, 0.5f}, {-1,0,0}, {1,0}},
//     {{-0.5f, 0.5f, 0.5f}, {-1,0,0}, {1,1}},
//     {{-0.5f, 0.5f,-0.5f}, {-1,0,0}, {0,1}},
//     // +X (right)
//     {{ 0.5f,-0.5f, 0.5f}, {1,0,0}, {0,0}},
//     {{ 0.5f,-0.5f,-0.5f}, {1,0,0}, {1,0}},
//     {{ 0.5f, 0.5f,-0.5f}, {1,0,0}, {1,1}},
//     {{ 0.5f, 0.5f, 0.5f}, {1,0,0}, {0,1}},
//     // +Y (top)
//     {{-0.5f, 0.5f, 0.5f}, {0,1,0}, {0,0}},
//     {{ 0.5f, 0.5f, 0.5f}, {0,1,0}, {1,0}},
//     {{ 0.5f, 0.5f,-0.5f}, {0,1,0}, {1,1}},
//     {{-0.5f, 0.5f,-0.5f}, {0,1,0}, {0,1}},
//     // -Y (bottom)
//     {{-0.5f,-0.5f,-0.5f}, {0,-1,0}, {0,0}},
//     {{ 0.5f,-0.5f,-0.5f}, {0,-1,0}, {1,0}},
//     {{ 0.5f,-0.5f, 0.5f}, {0,-1,0}, {1,1}},
//     {{-0.5f,-0.5f, 0.5f}, {0,-1,0}, {0,1}},
// };
static struct Vertex cube0_vertices[] = {

	{{-0.5f,-0.5f, 0.5f}, {0,0,1}, {0,0}},
    {{ 0.5f,-0.5f, 0.5f}, {0,0,1}, {1,0}},
    {{ 0.5f, 0.5f, 0.5f}, {0,0,1}, {1,1}},
    {{-0.5f, 0.5f, 0.5f}, {0,0,1}, {0,1}},


    {{ 0.5f,-0.5f,-0.5f}, {0,0,-1}, {0,0}},
    {{-0.5f,-0.5f,-0.5f}, {0,0,-1}, {1,0}},
    {{-0.5f, 0.5f,-0.5f}, {0,0,-1}, {1,1}},
    {{ 0.5f, 0.5f,-0.5f}, {0,0,-1}, {0,1}},

    

    {{-0.5f,-0.5f,-0.5f}, {-1,0,0}, {0,0}},
    {{-0.5f,-0.5f, 0.5f}, {-1,0,0}, {1,0}},
    {{-0.5f, 0.5f, 0.5f}, {-1,0,0}, {1,1}},
    {{-0.5f, 0.5f,-0.5f}, {-1,0,0}, {0,1}},


    {{ 0.5f,-0.5f, 0.5f}, {1,0,0}, {0,0}},
    {{ 0.5f,-0.5f,-0.5f}, {1,0,0}, {1,0}},
    {{ 0.5f, 0.5f,-0.5f}, {1,0,0}, {1,1}},
    {{ 0.5f, 0.5f, 0.5f}, {1,0,0}, {0,1}},

    {{-0.5f, 0.5f, 0.5f}, {0,1,0}, {0,0}},
    {{ 0.5f, 0.5f, 0.5f}, {0,1,0}, {1,0}},
    {{ 0.5f, 0.5f,-0.5f}, {0,1,0}, {1,1}},
    {{-0.5f, 0.5f,-0.5f}, {0,1,0}, {0,1}},


    {{-0.5f,-0.5f,-0.5f}, {0,-1,0}, {0,0}},
    {{ 0.5f,-0.5f,-0.5f}, {0,-1,0}, {1,0}},
    {{ 0.5f,-0.5f, 0.5f}, {0,-1,0}, {1,1}},
    {{-0.5f,-0.5f, 0.5f}, {0,-1,0}, {0,1}},
};
static uint32_t cube0_indices[] = {
    // front
    0, 1, 2, 2, 3, 0,
    // back
    4, 5, 6, 6, 7, 4,
    // left
    8, 9,10,10,11, 8,
    // right
    12,13,14,14,15,12,
    // top
    16,17,18,18,19,16,
    // bottom
    20,21,22,22,23,20
};

static void create_Instance0(
	struct ApplicationContext * app,
	uint32_t indices_count,
	uint32_t indices_offset,
	uint32_t vertices_offset
);

void Asset0_init(struct ApplicationContext * app){
	PRINT_FNAME;
	uint32_t indices_offset = app->renderer.buffer_index_used;
	uint32_t indices_count = ARR_LEN(cube0_indices);
	Renderer_AppendToIndexBuffer(
		&app->renderer,
		cube0_indices, 
		indices_count
	);

	uint32_t vertices_offset = app->renderer.buffer_vertex_used;
	uint32_t vertices_count = ARR_LEN(cube0_vertices);
	Renderer_AppendToVertexBuffer(
		&app->renderer,
		cube0_vertices, 
		vertices_count
	);

	create_Instance0(app,indices_count,indices_offset,vertices_offset);


	
}

static void create_Instance0(
	struct ApplicationContext * app,
	uint32_t indices_count,
	uint32_t indices_offset,
	uint32_t vertices_offset
){
	

	
	
	struct Mesh * mesh0 = Scene_NewMesh(&app->scene);
	uint32_t mesh_index = app->scene.mesh_count - 1;

	
	mesh0->index_offset = indices_offset;
	mesh0->index_count = indices_count;
	mesh0->vertex_offset = vertices_offset;
	// mesh0->vertex_count = vertices_count;


	struct MaterialData * material0 = Scene_NewMaterial(&app->scene);
	uint32_t material_idx = app->scene.material_count - 1;

	
	GLM_VEC4_SET(material0->base_color_factor, 0.0, 0.0, 1.0, 0.5);

	
	struct Texture * texture0 = Renderer_NewTexture(
		&app->renderer, 
		image_png0, 
		sizeof(image_png0)
	);
	uint32_t texture_idx = app->renderer.texture_cnt - 1;
	material0->base_color_texture_idx = texture_idx;

	
	struct InstanceData *inst = Scene_NewInstanceData(&app->scene);
	uint32_t instance_index = app->scene.instance_count - 1;
	mesh0->instance_cnt = 1;
	mesh0->instance_offset = instance_index;
	
	glm_mat4_identity(inst->model);
	inst->camera_idx= 0;
	inst->material_idx = material_idx;

	
	for(int i=0; i<MAX_TEXTURES ;i++)
	{
		Descriptor_SetTextureToDescriptorInfoArray(texture0->image.view, texture0->sampler,i);
	}

	struct DescriptorContext * context = Descriptor_GetContext();
	Descriptor_UpdateTextureDescriptors(
		app->renderer.desc_set_samplers, 
		context->descriptor_image_info_textures, 
		MAX_TEXTURES,
		0
	);

	
	app->scene.global_data.instance_cnt = app->scene.instance_count;
}
