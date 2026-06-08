#include <assert.h>
#include <stdint.h>

#include "../descriptor.h"
#include "../shader/common.h"
#include "../common.h"
#include "../vertex.h"
#include "../resource.h"
#include "../resource2.h"
#include "asset0.h"
#include "../loader/loader.h"

static uint8_t  image_png0[] = 
{
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7A, 0x7A, 0xF4, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x01, 0xD9, 0xC9, 0x2C, 0x7F, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xB1, 0x8F, 0x0B, 0xFC, 0x61, 0x05, 0x00, 0x00, 0x00, 0x20, 0x63, 0x48, 0x52, 0x4D, 0x00, 0x00, 0x7A, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00, 0xFA, 0x00, 0x00, 0x00, 0x80, 0xE8, 0x00, 0x00, 0x75, 0x30, 0x00, 0x00, 0xEA, 0x60, 0x00, 0x00, 0x3A, 0x98, 0x00, 0x00, 0x17, 0x70, 0x9C, 0xBA, 0x51, 0x3C, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x2E, 0x23, 0x00, 0x00, 0x2E, 0x23, 0x01, 0x78, 0xA5, 0x3F, 0x76, 0x00, 0x00, 0x00, 0x41, 0x49, 0x44, 0x41, 0x54, 0x58, 0xC3, 0xED, 0xD6, 0xC1, 0x09, 0x00, 0x30, 0x0C, 0x42, 0xD1, 0xA4, 0x3B, 0x9A, 0x21, 0x5D, 0xD2, 0x0E, 0x91, 0x43, 0x29, 0x7C, 0xEF, 0xC2, 0x03, 0x2F, 0xB6, 0xA4, 0xD4, 0x22, 0xB6, 0x37, 0xF5, 0x3A, 0xF5, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x4E, 0xB2, 0xFA, 0x03, 0x33, 0xC3, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xDF, 0x80, 0x0B, 0xBB, 0xA7, 0x09, 0x39, 0x81, 0xAB, 0x3D, 0x73, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

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
	struct Mess * ref,
	uint32_t indices_count,
	uint32_t indices_offset,
	uint32_t vertices_offset
);
/*
 createMesh
	*/
void Asset0_init(struct Mess * ref){
	PRINT_FNAME;

	uint32_t indices_offset = ref->gpu_objects.buffer_index_used;
	uint32_t indices_count = ARR_LEN(cube0_indices);

	assert(indices_offset + indices_count <= MAX_INDICES);
	
	ref->gpu_objects.buffer_index_used += indices_count;
	
	 Resource_AppendToIndexBuffer(
		ref->ref_device,
		&ref->gpu_objects.buffer_index, 
		cube0_indices, 
		indices_offset,
		indices_count
	);
	

	uint32_t vertices_offset = ref->gpu_objects.buffer_vertex_used;
	uint32_t vertices_count = ARR_LEN(cube0_vertices);
	assert(vertices_offset + vertices_count <= MAX_VERTICES);
	
	ref->gpu_objects.buffer_index_used += indices_count;

	
	
	Resource_AppendToVertexBuffer(
		ref->ref_device,
		&ref->gpu_objects.buffer_vertex, 
		cube0_vertices, 
		vertices_offset,
		vertices_count
	);

	create_Instance0(ref,indices_count,indices_offset,vertices_offset);


	
}



static void create_Instance0(
	struct Mess * ref,
	uint32_t indices_count,
	uint32_t indices_offset,
	uint32_t vertices_offset
){
	

	
	
	struct Mesh * mesh0 = Assets_NewMesh(ref);

	mesh0->index_offset = indices_offset;
	mesh0->index_count = indices_count;
	mesh0->vertex_offset = vertices_offset;
	// mesh0->vertex_count = vertices_count;


	struct MaterialData * material0 = Assets_NewMaterial(ref);
	uint32_t material_idx = ref->cpu_data.material_count - 1;

	
	GLM_VEC4_SET(material0->base_color_factor, 0.0, 0.0, 1.0, 0.5);

	
	struct Image * texture0 = Assets_NewTexture(
		ref, 
		image_png0, 
		sizeof(image_png0)
	);
	uint32_t texture_idx = ref->gpu_objects.texture_cnt - 1;
	material0->base_color_texture_idx = texture_idx;

	
	struct InstanceData *inst = Assets_NewInstanceData(ref);
	uint32_t instance_index = ref->cpu_data.instance_count - 1;
	mesh0->instance_cnt = 1;
	mesh0->instance_offset = instance_index;
	
	glm_mat4_identity(inst->model);
	inst->material_idx = material_idx;

	
	Descriptor_UpdateTextureDescriptors(
		ref->ref_device->logical_device, 
		ref->sets.set_textures, 
		0, 
		texture0->view, 
		0
	);

	// struct DescriptorContext * context = &ref->s;
		
	// for(int i=0; i<MAX_TEXTURES ;i++)
	// {
	// 	// Descriptor_SetTextureToDescriptorInfoArray(context,texture0->image.view, texture0->sampler,i);

	// 	Descriptor_UpdateTextureDescriptors(context,
	// 		app->renderer.desc_set_textures, 0, texture0->view, i);
	// }


	// Descriptor_UpdateTextureDescriptors(context,
	// 	app->renderer.desc_set_samplers, 
	// 	context->descriptor_image_info_textures, 
	// 	MAX_TEXTURES,
	// 	0
	// );

	
	ref->cpu_data.global_data.instance_cnt += 1;

	// app->scene.data->global_data.instance_cnt = app->scene.instance_count;
}






struct Mesh* Assets_NewMesh(struct Mess* ref) {
	assert(ref->cpu_data.mesh_count < MAX_MESHES);
	
	ref->cpu_data.meshes[ref->cpu_data.mesh_count].instance_cnt =  UNSET_VALUE;
	ref->cpu_data.meshes[ref->cpu_data.mesh_count].instance_offset =  UNSET_VALUE;
	return &ref->cpu_data.meshes[ref->cpu_data.mesh_count++];
}

struct Mesh* Assets_GetMesh(struct Mess* ref, uint32_t position) {
	assert(ref->cpu_data.mesh_count > position);

	return &ref->cpu_data.meshes[position];
}

struct MaterialData* Assets_NewMaterial(struct Mess* ref) {
	assert(ref->cpu_data.material_count < MAX_MATERIALS);
	ref->cpu_data.material_data[ref->cpu_data.material_count].base_color_texture_idx = UNSET_VALUE;
	ref->cpu_data.material_data[ref->cpu_data.material_count].metallic_roughness_texture_idx = UNSET_VALUE;	
	return &ref->cpu_data.material_data[ref->cpu_data.material_count++];
}

struct MaterialData* Assets_GetMaterial(struct Mess* ref, uint32_t position) {
	assert(ref->cpu_data.material_count > position);

	return &ref->cpu_data.material_data[position];
}

struct InstanceData* Assets_NewInstanceData(
	struct Mess* ref
){
	assert(ref->cpu_data.instance_count < MAX_INSTANCES);
	ref->cpu_data.instance_data[ref->cpu_data.instance_count].material_idx = UNSET_VALUE;
	return &ref->cpu_data.instance_data[ref->cpu_data.instance_count++];
}

struct InstanceData* Assets_GeInstanceData(
	struct Mess* ref, 
	uint32_t position
) {
	assert(ref->cpu_data.instance_count > position);

	return &ref->cpu_data.instance_data[position];
}



struct Image* Assets_NewTexture(
	struct Mess* ref, void* data, uint32_t size
) {
	PRINT_FNAME;

 	struct ImageData imageData;

	struct Image* texture = &ref->gpu_objects.textures[ref->gpu_objects.texture_cnt++];
	Loader_LoadImageDataFromMemory(data, size, &imageData);
	Resource2_CreateTexture(ref->ref_device,imageData.data, imageData.width, imageData.height,
		VK_FORMAT_R8G8B8A8_SRGB, texture);
	Loader_FreeImageData(&imageData);
	
	return texture;
}

struct Image* assets_GetTexture(
	struct Mess* ref, uint32_t position
) {
	assert(ref->gpu_objects.texture_cnt > position);

	return &ref->gpu_objects.textures[position];
}
