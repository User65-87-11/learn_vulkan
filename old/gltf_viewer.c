


//

#include "cglm/vec3.h"
#include <vulkan/vulkan_core.h>




#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "cglm/cglm.h"
#include "cglm/cam.h"
#include "cglm/types.h"
#include "cglm/mat4.h"
#include "cglm/util.h"
#include "cglm/affine-pre.h"

// #define TINYOBJ_LOADER_C_IMPLEMENTATION
// #include "obj_loader/tinyobj_loader_c.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"



#include <math.h>

#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#include <stdlib.h>
#else
#include <unistd.h>   // for getcwd
#endif
#include <limits.h>   // for PATH_MAX


#define PRINT_FNAME printf("Call to: %s\n",__FUNCTION__)

#define ARR_LEN(A) sizeof(A)/sizeof(*A)

#define EXIT_CLEAN(msg)\
	do{\
		printf("ERROR: %s\n",msg);\
		exit(1);\
	}while(0)\



void loadModel(char *fname){
	PRINT_FNAME;

	printf("load model: %s\n",fname);
	/*
	cgltf_load_buffers(const cgltf_options *options, cgltf_data *data, const char *gltf_path) -> cgltf_result

	*/
	const cgltf_options options={};
	cgltf_data *data;
	uint32_t data_size;
	const char * gltf_path = fname;

	cgltf_result res;

	/**
cgltf_result cgltf_parse_file(const cgltf_options* options, const
 * char* path, cgltf_data** out_data)
  */

  	res = cgltf_parse_file(&options,fname,&data);


	// res = cgltf_load_buffers(&options,data,gltf_path);

	if(res != cgltf_result_success)
	{
		printf("res %d\n",res);
		EXIT_CLEAN("Cant load buffers :(");
	}

	for(int i=0;i<data->buffers_count;i++){
		printf("Buffer:%s\n",data->buffers[i].uri);
		printf("Buffer.size:%lld\n",data->buffers[i].size);
		printf("Buffer.name:%s\n",data->buffers[i].name);
		printf("Buffer.uri:%s\n",data->buffers[i].uri);
		printf("Buffer.data:%p\n",data->buffers[i].data);
	}

	/*
	typedef enum cgltf_buffer_view_type
{
	cgltf_buffer_view_type_invalid,
	cgltf_buffer_view_type_indices,
	cgltf_buffer_view_type_vertices,
	cgltf_buffer_view_type_max_enum
} cgltf_buffer_view_type;
	*/


	// for(int i=0;i<data->buffer_views_count;i++){
	// 	printf("Buffer views:");
	// 	printf(" .buffer ptr:%p\n",data->buffer_views[i].buffer);
	// 	printf(" .data:%p\n",data->buffer_views[i].data);
	// 	printf(" .type:%d\n",data->buffer_views[i].type);
	// 	printf(" .size:%ld\n",data->buffer_views[i].size);
	// 	printf(" .offset:%ld\n",data->buffer_views[i].offset);
	// 	printf(" .stride:%ld\n",data->buffer_views[i].stride);
	// 	printf(" .data:%p\n",data->buffer_views[i].data);
	// }

	printf("Scene count: %d\n",data->scenes_count);
	for(int i=0;i<data->scenes_count;i++)
	{

		printf("Node count: %d\n",data->scenes[i].nodes_count);
		for(int j=0;j<data->scenes[i].nodes_count;j++)
		{
			printf("Node name: %s\n",data->scenes[i].nodes[j]->name);
		}
	}

	printf("Lights %d:\n",data->lights_count);
	for(int i=0; i < data->lights_count;i++){
		printf("[%d] name: %s type:%d \n",i,data->lights[i].name,data->lights[i].type);
		printf("\t color:%f,%f,%f\n",data->lights[i].color[0],data->lights[i].color[1],data->lights[i].color[2]);
		printf("\t intens:%f\n",data->lights[i].intensity);
		printf("\t range:%f\n",data->lights[i].range);
	}

	printf("Meshes cnt [%d]:\n",data->meshes_count);
	for(int i=0;i<data->meshes_count;i++){

		printf("Meshes \n");
		printf(" .name:%s\n",data->meshes[i].name);

		printf(" .primitives_count:%ld\n",data->meshes[i].primitives_count);


		for(int j=0;j<data->meshes[i].primitives_count;j++){

			printf(" Primitives :\n");
			printf("  .attributes_count:%ld\n",data->meshes[i].primitives[j].attributes_count);
			printf("  .material:%p\n",data->meshes[i].primitives[j].material);

			//prim->indices → pointer to a cgltf_accessor
			printf("  .indices:%p\n",data->meshes[i].primitives[j].indices);

			if(data->meshes[i].primitives[j].indices)
			{
				cgltf_accessor * accessor = data->meshes[i].primitives[j].indices;
				printf("    .cnt:%ld\n",accessor->count);
				printf("    .type:%d\n",accessor->type);
				printf("    .buffer offset:%ld\n",accessor->buffer_view->offset);
				printf("    .offset:%ld\n",accessor->offset);
				printf("    .stride:%ld\n",accessor->stride);
			}

			for(int k=0;k<data->meshes[i].primitives[j].attributes_count;k++)
			{
				printf("  Attributes :");
				printf("   .name:%s\n",data->meshes[i].primitives[j].attributes[k].name);
				printf("   .data:%p\n",data->meshes[i].primitives[j].attributes[k].data);
				printf("   .type:%d\n",data->meshes[i].primitives[j].attributes[k].type);
				printf("   .index:%d\n",data->meshes[i].primitives[j].attributes[k].index);

				cgltf_accessor * accessor = data->meshes[i].primitives[j].attributes[k].data;
				cgltf_attribute_type attribType =  data->meshes[i].primitives[j].attributes[k].type;

				if(attribType == cgltf_attribute_type_position)
				{
					printf("  .vertex position\n");
					printf("    .count:%ld\n",accessor->count);
					printf("    .buffer_view->buffer:%p\n",accessor->buffer_view->buffer);
					printf("    .buffer_view->offset:%ld\n",accessor->buffer_view->offset);
					printf("    .accessor->offset:%ld\n",accessor->offset);
					printf("    .stride:%ld\n",accessor->stride);
					printf("    .cgltf_type:%d\n",accessor->type);
				}

				if(attribType == cgltf_attribute_type_texcoord)
				{
					printf("  .tex coords\n");
					printf("    .count:%ld\n",accessor->count);
					printf("    .buffer_view->buffer:%p\n",accessor->buffer_view->buffer);
					printf("    .buffer_view->offset:%ld\n",accessor->buffer_view->offset);
					printf("    .accessor->offset:%ld\n",accessor->offset);
					printf("    .stride:%ld\n",accessor->stride);
					printf("    .cgltf_type:%d\n",accessor->type);
				}


			}
			printf(" .primitives_count:%ld\n",data->meshes[i].primitives_count);
		}
		break;
	}

	if(data->buffers_count > 0){


		char url_path[256] ;

		uint32_t n = snprintf(url_path, sizeof(url_path), "models_gltf/%s", data->buffers[0].uri);

		if (n >= (uint32_t)sizeof(url_path)) {


			EXIT_CLEAN("url_path Overflow!");
		}


		printf("Buffer.path:%s\n",url_path);

		cgltf_load_buffers(&options,data,url_path);

		printf("Buffer.data:%p\n",data->buffers[0].data);
		printf("Buffer.size:%ld\n",data->buffers[0].size);

	}

	printf("data.data_extensions_count. %ld\n",data->data_extensions_count);
	printf("data.buffer_views_count %ld\n",data->buffer_views_count);
	printf("data.animations_count %ld\n",data->animations_count);
	printf("data.images_count %ld\n",data->images_count);
	printf("data.materials_count %ld\n",data->materials_count);
	printf("data.nodes_count %ld\n",data->nodes_count);
	printf("data.buffers_count %ld\n",data->buffers_count);
	printf("data.lights_count %ld\n",data->lights_count);
	printf("data.meshes_count %ld\n",data->meshes_count);



	//LOAD MODEL

	/**
	typedef enum cgltf_attribute_type
{
	cgltf_attribute_type_invalid,
	cgltf_attribute_type_position,
	cgltf_attribute_type_normal,
	cgltf_attribute_type_tangent,
	cgltf_attribute_type_texcoord,
	cgltf_attribute_type_color,
	cgltf_attribute_type_joints,
	cgltf_attribute_type_weights,
	cgltf_attribute_type_custom,
	cgltf_attribute_type_max_enum
} cgltf_attribute_type;

typedef enum cgltf_type
{
	cgltf_type_invalid,
	cgltf_type_scalar,
	cgltf_type_vec2,
	cgltf_type_vec3,
	cgltf_type_vec4,
	cgltf_type_mat2,
	cgltf_type_mat3,
	cgltf_type_mat4,
	cgltf_type_max_enum
} cgltf_type;

	*/
	// cgltf_primitive *primitive = &data->meshes[0].primitives[0];


	// cgltf_accessor *acc_idx = primitive->indices;
	// cgltf_accessor *acc_vert = NULL;

	// for(int i=0;i < primitive->attributes_count;i++){

	// 	cgltf_attribute attr =  primitive->attributes[i];
	// 	if(attr.type == cgltf_attribute_type_position)
	// 	{

	// 		cgltf_accessor* accessor = attr.data;

	// 		cgltf_buffer_view* view = accessor->buffer_view;

	// 		uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;
	// 		size_t stride = accessor->stride ? accessor->stride : sizeof(float) * 3;

	// 		size_t verticesNum = accessor->count;

	// 		printf("verticesNum: %d\n",verticesNum);
	// 		printf("stride: %ld\n",stride);



	// 	}


	// }


	// for(int i=0;i<primitive->attributes_count;i++){

	// 	cgltf_attribute attr =  primitive->attributes[i];

	// 	if(attr.type == cgltf_attribute_type_normal)
	// 	{
	// 		assert(attr.data->type == cgltf_type_vec3);
	// 		assert(attr.data->component_type == cgltf_component_type_r_32f);

	// 		cgltf_accessor* accessor = attr.data;
	// 		cgltf_buffer_view* view = accessor->buffer_view;

	// 		uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;
	// 		size_t stride = accessor->stride ? accessor->stride : sizeof(float) * 2;

	// 		size_t verticesNum = accessor->count;


	// 		for (size_t d = 0; d < verticesNum; d++) {
	// 			float* data = (float*)(base + d * stride);

	// 		}
	// 		break;
	// 	}
	// }



	// for(int i=0;i<primitive->attributes_count;i++){

	// 	cgltf_attribute attr =  primitive->attributes[i];

	// 	if(attr.type == cgltf_attribute_type_texcoord)
	// 	{
	// 		assert(attr.data->type == cgltf_type_vec2);
	// 		assert(attr.data->component_type == cgltf_component_type_r_32f);

	// 		cgltf_accessor* accessor = attr.data;
	// 		cgltf_buffer_view* view = accessor->buffer_view;

	// 		uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;
	// 		size_t stride = accessor->stride ? accessor->stride : sizeof(float) * 2;

	// 		// verticesNum = accessor->count;

	// 			size_t verticesNum = accessor->count;

	// 		for (size_t d = 0; d < verticesNum; d++) {
	// 			float* data = (float*)(base + d * stride);

	// 		}
	// 		break;
	// 	}
	// }

	// if(primitive->indices)
	// {

	// 	// printf("primitive->indices->component_type %d\n",primitive->indices->component_type);
	// 	// assert(primitive->indices->component_type == cgltf_component_type_r_16u);


	// 	cgltf_accessor* accessor = primitive->indices;
	// 	cgltf_buffer_view* view = accessor->buffer_view;


	// 	uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;

	// 	size_t stride = accessor->stride;

	// 	if (stride == 0) {
	// 		stride = cgltf_component_size(accessor->component_type);
	// 	}

	// 	size_t indicesNum = primitive->indices->count;



	// 	for(int d=0;d<indicesNum;d++){

	// 		uint8_t* ptr = base + d * stride;

	// 		 switch (accessor->component_type) {
	// 			case cgltf_component_type_r_16u:
	// 			 		printf("datatype:cgltf_component_type_r_16u\n");
	// 				break;
	// 			case cgltf_component_type_r_32u:
	// 			 		printf("datatype:cgltf_component_type_r_32u\n");
	// 				break;
	// 			case cgltf_component_type_r_8u:
	// 			 		printf("datatype:cgltf_component_type_r_8u\n");
	// 				break;
	// 			default:{
	// 					printf("datatype: %d\n",accessor->component_type);
	// 					EXIT_CLEAN("UNSUPPORTED INDEX DATA TYPE");
	// 				}
	// 				break;
	// 		}

	// 	}

	// }



	cgltf_free(data);



}


int main(int argc, char ** argv){

	if (argc != 2)
	{

		printf("require file path\n");
		return 1;
	}

	loadModel(argv[1]);

	return 0;
}
