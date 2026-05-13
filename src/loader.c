#include "src/scene.h"
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "loader.h"
#include "util/common.h"


void Loader_LoadImageDataFromFile(char * path, struct ImageData * out){
	
 	int w,h,c;

	stbi_uc *pixels = stbi_load(
		path,	
		&w, 
		&h, 
		&c, 
		STBI_rgb_alpha
	);
	
	if (!pixels) {
		EXIT_CLEAN("failed to load texture image!");
	}
	out->width = w;
	out->height = h;
	out->channels = c;
	out->data = pixels;
	out->size = w *h * 4;
}
void Loader_LoadImageDataFromMemory(void * data, uint32_t size , struct ImageData * out){
	
	// int texWidth, texHeight, texChannels;
	int w,h,c;
	stbi_uc *pixels = stbi_load_from_memory(
		data,
		size, 	
		&w, 
		&h, 
		&c,
		STBI_rgb_alpha
	);
	
	if (!pixels) {
		EXIT_CLEAN("failed to load texture image!");
	}

	out->width = w;
	out->height = h;
	out->channels = c;
	out->data = pixels;
	out->size = size;
}
void Loader_FreeImageData(struct ImageData * image_data){

	if(image_data->data != NULL)
	{
		stbi_image_free(image_data->data);
		image_data->data = NULL;
	}
}


void Loader_LoadGLTF(char *path, struct Scene * out){
	// void loadModel(char *fname, uint32_t *indicesNum, uint32_t **indices,
 //               uint32_t *verticesNum, struct Vertex **vertices) 
	// {
	
  PRINT_FNAME;

  printf("load model: %s\n", fname);

  const cgltf_options options = {};
  cgltf_data *data;
  uint32_t data_size;
  const char *gltf_path = fname;

  cgltf_result res;

  res = cgltf_parse_file(&options, fname, &data);

  if (res != cgltf_result_success) {
    printf("res %d\n", res);
    EXIT_CLEAN("Cant load buffers :(");
  }

  for (int i = 0; i < data->buffers_count; i++) {
    printf("Buffer:%s\n", data->buffers[i].uri);
    printf("Buffer.size:%lld\n", data->buffers[i].size);
    printf("Buffer.name:%s\n", data->buffers[i].name);
    printf("Buffer.uri:%s\n", data->buffers[i].uri);
    printf("Buffer.data:%p\n", data->buffers[i].data);
  }

  printf("Scene count: %d\n", data->scenes_count);
  for (int i = 0; i < data->scenes_count; i++) {

    printf("Node count: %d\n", data->scenes[i].nodes_count);
    for (int j = 0; j < data->scenes[i].nodes_count; j++) {
      printf("Node name: %s\n", data->scenes[i].nodes[j]->name);
    }
  }

  for (int i = 0; i < data->meshes_count; i++) {

    printf("Meshes :");
    printf(" .name:%s\n", data->meshes[i].name);

    printf(" .primitives_count:%ld\n", data->meshes[i].primitives_count);

    for (int j = 0; j < data->meshes[i].primitives_count; j++) {

      printf(" Primitives :\n");
      printf("  .attributes_count:%ld\n",
             data->meshes[i].primitives[j].attributes_count);
      printf("  .material:%p\n", data->meshes[i].primitives[j].material);

      printf("  .indices:%p\n", data->meshes[i].primitives[j].indices);

      if (data->meshes[i].primitives[j].indices) {
        cgltf_accessor *accessor = data->meshes[i].primitives[j].indices;
        printf("    .cnt:%ld\n", accessor->count);
        printf("    .type:%d\n", accessor->type);
        printf("    .buffer offset:%ld\n", accessor->buffer_view->offset);
        printf("    .offset:%ld\n", accessor->offset);
        printf("    .stride:%ld\n", accessor->stride);
      }

      for (int k = 0; k < data->meshes[i].primitives[j].attributes_count; k++) {
        printf("  Attributes :");
        printf("   .name:%s\n",
               data->meshes[i].primitives[j].attributes[k].name);
        printf("   .data:%p\n",
               data->meshes[i].primitives[j].attributes[k].data);
        printf("   .type:%d\n",
               data->meshes[i].primitives[j].attributes[k].type);
        printf("   .index:%d\n",
               data->meshes[i].primitives[j].attributes[k].index);

        cgltf_accessor *accessor =
            data->meshes[i].primitives[j].attributes[k].data;
        cgltf_attribute_type attribType =
            data->meshes[i].primitives[j].attributes[k].type;

        if (attribType == cgltf_attribute_type_position) {
          printf("  .vertex position\n");
          printf("    .count:%ld\n", accessor->count);
          printf("    .buffer_view->buffer:%p\n",
                 accessor->buffer_view->buffer);
          printf("    .buffer_view->offset:%ld\n",
                 accessor->buffer_view->offset);
          printf("    .accessor->offset:%ld\n", accessor->offset);
          printf("    .stride:%ld\n", accessor->stride);
          printf("    .cgltf_type:%d\n", accessor->type);
        }

        if (attribType == cgltf_attribute_type_texcoord) {
          printf("  .tex coords\n");
          printf("    .count:%ld\n", accessor->count);
          printf("    .buffer_view->buffer:%p\n",
                 accessor->buffer_view->buffer);
          printf("    .buffer_view->offset:%ld\n",
                 accessor->buffer_view->offset);
          printf("    .accessor->offset:%ld\n", accessor->offset);
          printf("    .stride:%ld\n", accessor->stride);
          printf("    .cgltf_type:%d\n", accessor->type);
        }
      }
      printf(" .primitives_count:%ld\n", data->meshes[i].primitives_count);
    }
    break;
  }

  if (data->buffers_count > 0) {

    char url_path[256];

    uint32_t n = snprintf(url_path, sizeof(url_path), "models_gltf/%s",
                          data->buffers[0].uri);

    if (n >= (uint32_t)sizeof(url_path)) {

      EXIT_CLEAN("url_path Overflow!");
    }

    printf("Buffer.path:%s\n", url_path);

    cgltf_load_buffers(&options, data, url_path);

    printf("Buffer.data:%p\n", data->buffers[0].data);
    printf("Buffer.size:%ld\n", data->buffers[0].size);
  }

  printf("data.data_extensions_count. %ld\n", data->data_extensions_count);
  printf("data.buffer_views_count %ld\n", data->buffer_views_count);
  printf("data.animations_count %ld\n", data->animations_count);
  printf("data.images_count %ld\n", data->images_count);
  printf("data.materials_count %ld\n", data->materials_count);
  printf("data.nodes_count %ld\n", data->nodes_count);
  printf("data.buffers_count %ld\n", data->buffers_count);
  printf("data.lights_count %ld\n", data->lights_count);
  printf("data.meshes_count %ld\n", data->meshes_count);

  cgltf_primitive *primitive = &data->meshes[0].primitives[0];

  cgltf_accessor *acc_idx = primitive->indices;
  cgltf_accessor *acc_vert = NULL;

  for (int i = 0; i < primitive->attributes_count; i++) {

    cgltf_attribute attr = primitive->attributes[i];
    if (attr.type == cgltf_attribute_type_position) {

      cgltf_accessor *accessor = attr.data;

      cgltf_buffer_view *view = accessor->buffer_view;

      uint8_t *base =
          (uint8_t *)view->buffer->data + view->offset + accessor->offset;
      uint32_t stride = accessor->stride ? accessor->stride : sizeof(float) * 3;

      *verticesNum = accessor->count;
      *vertices = malloc(sizeof(struct Vertex) * (*verticesNum));

      printf("verticesNum: %d\n", *verticesNum);
      printf("stride: %ld\n", stride);

      for (uint32_t d = 0; d < *verticesNum; d++) {
        float *data = (float *)(base + d * stride);

        struct Vertex *v = *vertices;

        v[d].pos[0] = data[0];
        v[d].pos[1] = data[1];
        v[d].pos[2] = data[2];
      }

      break;
    }
  }

  for (int i = 0; i < primitive->attributes_count; i++) {

    cgltf_attribute attr = primitive->attributes[i];

    if (attr.type == cgltf_attribute_type_normal) {
      assert(attr.data->type == cgltf_type_vec3);
      assert(attr.data->component_type == cgltf_component_type_r_32f);

      cgltf_accessor *accessor = attr.data;
      cgltf_buffer_view *view = accessor->buffer_view;

      uint8_t *base =
          (uint8_t *)view->buffer->data + view->offset + accessor->offset;
      uint32_t stride = accessor->stride ? accessor->stride : sizeof(float) * 2;

      for (uint32_t d = 0; d < *verticesNum; d++) {
        float *data = (float *)(base + d * stride);

        struct Vertex *v = *vertices;

        v[d].norm[0] = data[0];
        v[d].norm[1] = data[1];
        v[d].norm[2] = data[2];
      }
      break;
    }
  }

  for (int i = 0; i < primitive->attributes_count; i++) {

    cgltf_attribute attr = primitive->attributes[i];

    if (attr.type == cgltf_attribute_type_texcoord) {
      assert(attr.data->type == cgltf_type_vec2);
      assert(attr.data->component_type == cgltf_component_type_r_32f);

      cgltf_accessor *accessor = attr.data;
      cgltf_buffer_view *view = accessor->buffer_view;

      uint8_t *base =
          (uint8_t *)view->buffer->data + view->offset + accessor->offset;
      uint32_t stride = accessor->stride ? accessor->stride : sizeof(float) * 2;

      for (uint32_t d = 0; d < *verticesNum; d++) {
        float *data = (float *)(base + d * stride);

        struct Vertex *v = *vertices;

        v[d].texCoords[0] = data[0];
        v[d].texCoords[1] = data[1];
      }
      break;
    }
  }

  if (primitive->indices) {

    cgltf_accessor *accessor = primitive->indices;
    cgltf_buffer_view *view = accessor->buffer_view;

    uint8_t *base =
        (uint8_t *)view->buffer->data + view->offset + accessor->offset;

    uint32_t stride = accessor->stride;

    if (stride == 0) {
      stride = cgltf_component_size(accessor->component_type);
    }

    *indicesNum = primitive->indices->count;

    printf("indices num: %d\n", *indicesNum);
    *indices = malloc(sizeof(uint32_t) * (*indicesNum));

    for (int d = 0; d < *indicesNum; d++) {

      uint8_t *ptr = base + d * stride;

      uint32_t *ind = *indices;

      switch (accessor->component_type) {
      case cgltf_component_type_r_16u:
        ind[d] = *(uint16_t *)ptr;
        break;
      case cgltf_component_type_r_32u:
        ind[d] = *(uint32_t *)ptr;
        break;
      case cgltf_component_type_r_8u:
        ind[d] = *(uint8_t *)ptr;
        break;
      default: {
        printf("datatype: %d\n", accessor->component_type);
        EXIT_CLEAN("UNSUPPORTED INDEX DATA TYPE");
      } break;
      }
    }
  }

  cgltf_free(data);

}