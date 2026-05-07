

#include <time.h>
#include <vulkan/vulkan_core.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "cglm/cam.h"
#include "cglm/cglm.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/util.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

#include <math.h>

#include <stdio.h>

#include <limits.h>

#include "shaders/shader_inc.glsl"

#include "util/gm_array.h"
#include "util/gm_list.h"
#include "util/common.h"
#include "instance.h"
#include "platform.h"



// #define PRINT_FNAME printf("Call to: %s\n", __FUNCTION__)

// #define ARR_LEN(A) sizeof(A) / sizeof(A[0])

// #define EXIT_CLEAN(msg)                                                        \
//   do {                                                                         \
//     printf("ERROR: %s\n", msg);                                                \
//     cleanup();                                                                 \
//     exit(1);                                                                   \
//   } while (0)

// #define GLM_VEC3_COPY(dst, src)                                                \
//   do {                                                                         \
//     dst[0] = src[0];                                                           \
//     dst[1] = src[1];                                                           \
//     dst[2] = src[2];                                                           \
//   } while (0)

// #define GLM_MAT4_COPY(dst, src) memcpy(dst, src, sizeof(mat4))

// #define GLM_VEC4_COPY(dst, src)                                                \
//   do {                                                                         \
//     dst[0] = src[0];                                                           \
//     dst[1] = src[1];                                                           \
//     dst[2] = src[2];                                                           \
//     dst[3] = src[3];                                                           \
//   } while (0)

// #define GLM_VEC4_SET(dst, a, b, c, d)                                          \
//   do {                                                                         \
//     dst[0] = a;                                                                \
//     dst[1] = b;                                                                \
//     dst[2] = c;                                                                \
//     dst[3] = d;                                                                \
//   } while (0)

// #define GLM_VEC3_SET(dst, a, b, c)                                             \
//   do {                                                                         \
//     dst[0] = a;                                                                \
//     dst[1] = b;                                                                \
//     dst[2] = c;                                                                \
//   } while (0)




struct Pipeline {

  char *frag_path;

  char *vert_path;

  VkShaderModule shaderModuleFrag;

  VkShaderModule shaderModuleVert;

  VkPipelineLayout pipelineLayout;

  VkPipeline graphicsPipeline;

  uint32_t colorAttachmentCount;

  VkRenderingInfo renderingInfo;
};




struct Entity {
  uint32_t idx;
  struct Range range_vertex;
  struct Range range_index;
};

struct Frame {

  VkCommandBuffer graphicsCommandBuffers;

  VkSemaphore presentCompleteSemaphore;

  VkSemaphore renderFinishedSemaphore;

  VkFence inFlightFence;


};

uint8_t texture0[] = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7A, 0x7A, 0xF4, 0x00, 0x00, 0x00,
    0x01, 0x73, 0x52, 0x47, 0x42, 0x01, 0xD9, 0xC9, 0x2C, 0x7F, 0x00, 0x00,
    0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xB1, 0x8F, 0x0B, 0xFC,
    0x61, 0x05, 0x00, 0x00, 0x00, 0x20, 0x63, 0x48, 0x52, 0x4D, 0x00, 0x00,
    0x7A, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00, 0xFA, 0x00, 0x00, 0x00,
    0x80, 0xE8, 0x00, 0x00, 0x75, 0x30, 0x00, 0x00, 0xEA, 0x60, 0x00, 0x00,
    0x3A, 0x98, 0x00, 0x00, 0x17, 0x70, 0x9C, 0xBA, 0x51, 0x3C, 0x00, 0x00,
    0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x2E, 0x23, 0x00, 0x00,
    0x2E, 0x23, 0x01, 0x78, 0xA5, 0x3F, 0x76, 0x00, 0x00, 0x00, 0x41, 0x49,
    0x44, 0x41, 0x54, 0x58, 0xC3, 0xED, 0xD6, 0xC1, 0x09, 0x00, 0x30, 0x0C,
    0x42, 0xD1, 0xA4, 0x3B, 0x9A, 0x21, 0x5D, 0xD2, 0x0E, 0x91, 0x43, 0x29,
    0x7C, 0xEF, 0xC2, 0x03, 0x2F, 0xB6, 0xA4, 0xD4, 0x22, 0xB6, 0x37, 0xF5,
    0x3A, 0xF5, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x4E,
    0xB2, 0xFA, 0x03, 0x33, 0xC3, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xC0, 0xDF, 0x80, 0x0B, 0xBB, 0xA7, 0x09, 0x39, 0x81, 0xAB, 0x3D, 0x73,
    0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
struct TextureRes tex_res0;

struct Vertex {

  vec3 pos;
  vec3 norm;
  vec2 texCoords;
};

struct Vertex cube0_vertices[] = {

    {{-0.5f, -0.5f, 0.5f}, {0, 0, 1}, {0, 0}},
    {{0.5f, -0.5f, 0.5f}, {0, 0, 1}, {1, 0}},
    {{0.5f, 0.5f, 0.5f}, {0, 0, 1}, {1, 1}},
    {{-0.5f, 0.5f, 0.5f}, {0, 0, 1}, {0, 1}},

    {{0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}},
    {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}},
    {{-0.5f, 0.5f, -0.5f}, {0, 0, -1}, {1, 1}},
    {{0.5f, 0.5f, -0.5f}, {0, 0, -1}, {0, 1}},

    {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 0}},
    {{-0.5f, -0.5f, 0.5f}, {-1, 0, 0}, {1, 0}},
    {{-0.5f, 0.5f, 0.5f}, {-1, 0, 0}, {1, 1}},
    {{-0.5f, 0.5f, -0.5f}, {-1, 0, 0}, {0, 1}},

    {{0.5f, -0.5f, 0.5f}, {1, 0, 0}, {0, 0}},
    {{0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1, 0}},
    {{0.5f, 0.5f, -0.5f}, {1, 0, 0}, {1, 1}},
    {{0.5f, 0.5f, 0.5f}, {1, 0, 0}, {0, 1}},

    {{-0.5f, 0.5f, 0.5f}, {0, 1, 0}, {0, 0}},
    {{0.5f, 0.5f, 0.5f}, {0, 1, 0}, {1, 0}},
    {{0.5f, 0.5f, -0.5f}, {0, 1, 0}, {1, 1}},
    {{-0.5f, 0.5f, -0.5f}, {0, 1, 0}, {0, 1}},

    {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 0}},
    {{0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 0}},
    {{0.5f, -0.5f, 0.5f}, {0, -1, 0}, {1, 1}},
    {{-0.5f, -0.5f, 0.5f}, {0, -1, 0}, {0, 1}},
};

uint32_t cube0_indices[] = {

    0,  1,  2,  2,  3,  0,

    4,  5,  6,  6,  7,  4,

    8,  9,  10, 10, 11, 8,

    12, 13, 14, 14, 15, 12,

    16, 17, 18, 18, 19, 16,

    20, 21, 22, 22, 23, 20};





VkFormat swapchainSurfaceFormat = -1;

uint32_t swapchainSurfaceColorSpace = -1;

VkSwapchainKHR swapchain = NULL;

uint32_t frameIndex = 0;

VkCommandPool graphicsCommnadPool = NULL;




uint32_t swapchain_img_cnt = 0;
// struct ImageRes swapchain_images[MAX_IMAGE_VIEWS] = {};




struct GmArray arrayPipelines;



struct Frame frames[MAX_FRAMES_IN_FLIGHT] = {};

/* END UNIFORM BUFFERS */




float startTime = 0.0f;
float deltaTime = 0.0f;
float lastTime = 0.0f;

struct Mesh *model0;

char *model_path_gltf = "models_gltf/viking_room.gltf";
char *model_path_bin = "models_gltf/viking_room.bin";
char *model_path_text = "models_gltf/viking_room.png";

// void cleanAllocation(struct Allocation *all);
// void cleanImageRes(struct ImageRes *img);
// void cleanBuffer(struct BufferRes *buff);
// void cleanTextureRes(struct TextureRes *tex);

void initVariables();

void freeVariables();



uint32_t createShaderFromFile(const char *path, uint8_t **buffer);

void loadModel(

    char *fname, uint32_t *indicesNum, uint32_t **indices,
    uint32_t *verticesNum, struct Vertex **vertices);

// void createUniformBuffers();

// void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
//                   VkMemoryPropertyFlags properties, VkBuffer *buffer,
//                   VkDeviceMemory *bufferMemory);

// uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

void recreateSwapChain();

void cleanup();

// void clearUniformBuffers();

// void createShaderDescriptorSetLayout();

// void createPickImage(

// );

// void generateMipmaps(VkImage *image, VkFormat imageFormat, int32_t texWidth,
//                      int32_t texHeight, uint32_t mipLevels);

// void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
//                  VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
//                  VkMemoryPropertyFlags properties, VkImage *image,
//                  VkDeviceMemory *imageMemory);

// void beginSingleTimeCommands(VkCommandBuffer commandBuffer);

// void endSingleTimeCommands(VkCommandBuffer commandBuffer);

// void transitionImageLayout(VkCommandBuffer cmdBuffer, VkImage *image,

//                            VkImageLayout oldLayout, VkImageLayout newLayout,

//                            VkAccessFlags srcAccessMask,
//                            VkAccessFlags dstAccessMask,

//                            VkPipelineStageFlags srcStageMask,
//                            VkPipelineStageFlags dstStageMask,

//                            VkImageAspectFlagBits aspectFlags,

//                            uint32_t mipLevels);

// void copyBufferToImage(VkBuffer *buffer, VkImage *image, uint32_t width,
//                        uint32_t height);

// void createTextureSamplerShadow(VkSampler *sampler);

// void createTextureSampler(VkSampler *sampler);


// VkFormat findDepthFormat();

bool hasStencilComponent(VkFormat format);

void processInput(GLFWwindow *window);

void createGraphicsPipeline(struct Pipeline *pipeline);

float rand_float(float from, float to) {

  return from + (to - from) * ((float)rand() / (float)RAND_MAX);
}
int32_t rand_int32(int32_t from, int32_t to) { return rand_float(from, to); }

// struct BufferRes *buffer_set(struct BufferRes *buffer, VkBuffer handle,
//                              uint32_t size, uint32_t offset,

//                              VkBufferUsageFlags usage, VkDeviceMemory memory,
//                              void *mapped

// ) {
//   buffer->handle = handle;
//   buffer->alloc.mapped = mapped;
//   buffer->alloc.memory = memory;
//   buffer->alloc.offset = offset;
//   buffer->alloc.size = size;
//   buffer->usage = usage;

//   return buffer;
// }



void initVariables() {
  PRINT_FNAME;

  gmListInit(&list_BufferRes);

  // gmListInit(&list_vertices);
  // gmListInit(&list_indices);

  gmArrayInit(&arrayPipelines, sizeof(struct Pipeline), 3,
              _Alignof(struct Pipeline));
  
  gmArrayInit(&array_data_instances, sizeof(struct Instance_ssbo),
              MAX_INSTANCES, _Alignof(struct Instance_ssbo));
  
  gmArrayInit(&array_data_materials, sizeof(struct Material_ssbo),
              MAX_INSTANCES, _Alignof(struct Material_ssbo));

 
}
void freeVariables() {

  gmListFree(&list_BufferRes);

  // gmListFree(&list_vertices);
  // gmListFree(&list_indices);



  gmArrayFree(&array_data_instances);
  gmArrayFree(&array_data_materials);


  gmArrayFree(&arrayPipelines);
}

void initMeshes() {
  PRINT_FNAME;

  uint32_t v_cnt = ARR_LEN(cube0_vertices);
  uint32_t i_cnt = ARR_LEN(cube0_indices);

  uint8_t *v_data = gmArrayNewN(&array_vertex_data, sizeof(cube0_vertices));
  memcpy(v_data, cube0_vertices, sizeof(cube0_vertices));

  uint8_t *i_data = gmArrayNewN(&array_index_data, sizeof(cube0_indices));
  memcpy(i_data, cube0_indices, sizeof(cube0_indices));

  struct Range *v_range = gmArrayNew(&array_vertex_ranges);
  v_range->cap = v_cnt;
  v_range->len = v_cnt;
  v_range->start = 0;

  struct Range *i_range = gmArrayNew(&array_index_ranges);
  i_range->cap = i_cnt;
  i_range->len = i_cnt;
  i_range->start = 0;

  model0 = gmArrayNew(&array_meshes);

  model0->range_vertex = *v_range;
  model0->range_index = *i_range;
  model0->index = 0;
}

void loadModel(char *fname, uint32_t *indicesNum, uint32_t **indices,
               uint32_t *verticesNum, struct Vertex **vertices) {
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

bool hasStencilComponent(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
         format == VK_FORMAT_D24_UNORM_S8_UINT;
}




void initGameObjects() {

  if (sizeof(struct Instance_ssbo) % 32 != 0 &&
      sizeof(struct Instance_ssbo) % 16 != 0) {
    printf("%d %d\n", sizeof(struct Instance_ssbo),
           sizeof(struct Instance_ssbo) % 16);
    EXIT_CLEAN("Instance_ssbo size is not mod 16");
  }
  if ((sizeof(struct Global_ubo) % 32) != 0 &&
      sizeof(struct Global_ubo) % 16 != 0) {
    printf("%d %d\n", sizeof(struct Global_ubo),
           sizeof(struct Global_ubo) % 16);
    EXIT_CLEAN("Global_ubo size is not mod 16");
  }
  if ((sizeof(struct Material_ssbo) % 32) != 0 &&
      sizeof(struct Material_ssbo) % 16 != 0) {
    printf("%d %d\n", sizeof(struct Material_ssbo),
           sizeof(struct Material_ssbo) % 16);
    EXIT_CLEAN("Material_ssbo size is not mod 16");
  }

  yaw = glm_deg(atan2(cameraFront[2], cameraFront[0]));
  pitch = glm_deg(asin(cameraFront[1]));

  {
    struct Instance_ssbo *inst0 = gmArrayNew(&array_data_instances);

    GLM_VEC4_SET(inst0->color, 1.0f, 1.0f, 1.0f, 1.0);
    inst0->color_factor = 0.1;
    glm_mat4_identity(inst0->model);
    inst0->tex_idx = 0;
  }

  {

    struct Global_ubo *glob = &data_globals;

    glm_perspective(glm_rad(45.0f),
                    (float)swapChainExtent.width / swapChainExtent.height, 0.1f,
                    40.0f, glob->proj);

    glob->proj[1][1] *= -1;

    glob->instance_cnt = array_data_instances.len;
  }
}

void cleanShaderBuffers() {
  struct GmNode *n = list_BufferRes.head;
  while (n != NULL) {
    struct BufferRes *ref = n->data;
    cleanBuffer(ref);
    n = n->next;
  }
  gmListFree(&list_BufferRes);
}



void createDescriptors() {
  PRINT_FNAME;
  uint32_t cnt_inst = MAX_FRAMES_IN_FLIGHT;
  uint32_t cnt_glob = MAX_FRAMES_IN_FLIGHT;
  uint32_t cnt_material = 1;
  uint32_t cnt_textures = 1;
  uint32_t max_sets = cnt_inst + cnt_glob + cnt_material + cnt_textures;

  VkDescriptorPoolSize poolSizes[] = {
      (VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                             .descriptorCount = MAX_TEXTURES},
      (VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                             .descriptorCount = MAX_FRAMES_IN_FLIGHT},
      (VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                             .descriptorCount =
                                 MAX_FRAMES_IN_FLIGHT * MAX_INSTANCES +
                                 MAX_MATERIALS},

  };

  VkDescriptorPoolCreateInfo poolInfo = {

      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      .pNext = NULL,
      .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
      .maxSets = max_sets,
      .poolSizeCount = ARR_LEN(poolSizes),
      .pPoolSizes = poolSizes};

  VkResult result =
      vkCreateDescriptorPool(device, &poolInfo, NULL, &descriptor_pool);

  {
    VkDescriptorSetLayoutBinding globalBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

    VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &globalBinding};

    VkResult res = vkCreateDescriptorSetLayout(device, &globalLayoutInfo, NULL,
                                               &descriptor_layout_global);

    if (res != VK_SUCCESS) {
      EXIT_CLEAN("vkCreateDescriptorSetLayout4 failed");
    }
  }

  {
    VkDescriptorSetLayoutBinding materialBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorCount = 1,
        .stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

    VkDescriptorSetLayoutCreateInfo materialLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &materialBinding};

    VkResult res = vkCreateDescriptorSetLayout(
        device, &materialLayoutInfo, NULL, &descriptor_layout_material);

    if (res != VK_SUCCESS) {
      EXIT_CLEAN("vkCreateDescriptorSetLayout  materialDescriptorSetLayout");
    }
  }

  {
    VkDescriptorSetLayoutBinding instanceBindings = {

        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorCount = 1,
        .stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

    VkDescriptorSetLayoutCreateInfo instanceLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &instanceBindings};

    VkResult res = vkCreateDescriptorSetLayout(
        device, &instanceLayoutInfo, NULL, &descriptor_layout_instance);

    if (res != VK_SUCCESS) {
      EXIT_CLEAN("vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
    }
  }

  {
    VkDescriptorSetLayoutBinding samplerBindings = {

        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = MAX_TEXTURES,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT};

    VkDescriptorSetLayoutCreateInfo samplerLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &samplerBindings};

    VkResult res = vkCreateDescriptorSetLayout(device, &samplerLayoutInfo, NULL,
                                               &descriptor_layout_sampler);

    if (res != VK_SUCCESS) {
      EXIT_CLEAN("vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
    }
  }

  {
    VkDescriptorSetLayout layouts[MAX_FRAMES_IN_FLIGHT];

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      layouts[i] = descriptor_layout_global;
    }

    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = descriptor_pool,
        .descriptorSetCount = MAX_FRAMES_IN_FLIGHT,
        .pSetLayouts = layouts};

    vkAllocateDescriptorSets(device, &allocInfo, descriptor_set_globals);
  }

  {
    VkDescriptorSetLayout layouts[MAX_FRAMES_IN_FLIGHT];

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      layouts[i] = descriptor_layout_instance;
    }

    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = descriptor_pool,
        .descriptorSetCount = MAX_FRAMES_IN_FLIGHT,
        .pSetLayouts = layouts};

    vkAllocateDescriptorSets(device, &allocInfo, descriptor_set_instances);
  }

  {

    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = descriptor_pool,
        .descriptorSetCount = 1,
        .pSetLayouts = &descriptor_layout_material};

    vkAllocateDescriptorSets(device, &allocInfo, &descriptor_set_materials);
  }

  {

    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = descriptor_pool,
        .descriptorSetCount = 1,
        .pSetLayouts = &descriptor_layout_sampler};

    vkAllocateDescriptorSets(device, &allocInfo, &descriptor_set_samplers);
  }
}







void createCommandPool() {

  PRINT_FNAME;

  VkCommandPoolCreateInfo commandPoolCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .queueFamilyIndex = queueFamilyIndeces[graphicsQueueFamilyArrayIndex],
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
  };

  vkCreateCommandPool(device, &commandPoolCreateInfo, NULL,
                      &graphicsCommnadPool);

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = graphicsCommnadPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    vkAllocateCommandBuffers(device, &allocInfo,
                             &frames[i].graphicsCommandBuffers);
  }

  VkCommandPoolCreateInfo commandPoolCreateInfo2 = (VkCommandPoolCreateInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .queueFamilyIndex = queueFamilyIndeces[transferQueueFamilyArrayIndex],
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
  };

  vkCreateCommandPool(device, &commandPoolCreateInfo2, NULL,
                      &transferCommnadPool);

  VkCommandBufferAllocateInfo allocInfo = (VkCommandBufferAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = transferCommnadPool,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1,

  };

  vkAllocateCommandBuffers(device, &allocInfo, &transferCommandBuffers);
}

void renderMainPass(VkCommandBuffer cmd, struct Frame *frame,
                    uint32_t imageIndex) {

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  VkClearValue clearDepth = {{{1.0f, 0}}};

  VkRenderingAttachmentInfo colorAttachmentsInfos[] = {

      colorAttachmentsInfos[0] =
          (VkRenderingAttachmentInfo){

              .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
              .imageView = swapchain_images[imageIndex].view,
              .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
              .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
              .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
              .clearValue = clearColor

          }

  };

  VkRenderingAttachmentInfo depthAttachmentInfo = {
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .imageView = frame->deapth.view,
      .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .clearValue = clearDepth};

  VkRenderingInfo renderingInfo = {
      .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
      .renderArea = {.offset = {0, 0}, .extent = swapChainExtent},

      .layerCount = 1,
      .colorAttachmentCount = ARR_LEN(colorAttachmentsInfos),
      .pColorAttachments = colorAttachmentsInfos,

      .pDepthAttachment = &depthAttachmentInfo,
  };

  vkCmdBeginRendering(cmd, &renderingInfo);

  struct Pipeline *pipeline = gmArrayGet(&arrayPipelines, 0);

  if (!pipeline || pipeline->graphicsPipeline == VK_NULL_HANDLE) {
    EXIT_CLEAN("ERROR in PIPELINE");
  }

  vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline->graphicsPipeline);

  VkViewport viewPort = {.x = 0,
                         .y = 0,
                         .width = swapChainExtent.width,
                         .height = swapChainExtent.height,
                         .minDepth = 0.0f,
                         .maxDepth = 1.0f};

  vkCmdSetViewport(cmd, 0, 1, &viewPort);

  VkRect2D scissor = {.extent = swapChainExtent, .offset = {0, 0}};
  vkCmdSetScissor(cmd, 0, 1, &scissor);

  VkDeviceSize offset = 0;

  vkCmdBindVertexBuffers(cmd, 0, 1, &vertex_buffer.handle, &offset);
  vkCmdBindIndexBuffer(cmd, index_buffer.handle, 0, VK_INDEX_TYPE_UINT32);

  /**

  VkDescriptorSet descriptor_set_instances [MAX_FRAMES_IN_FLIGHT];
VkDescriptorSet descriptor_set_globals [MAX_FRAMES_IN_FLIGHT];
VkDescriptorSet descriptor_set_materials;
VkDescriptorSet descriptor_set_samplers;
  */
  VkDescriptorSet dset[4] = {descriptor_set_globals[frameIndex],
                             descriptor_set_instances[frameIndex],
                             descriptor_set_materials, descriptor_set_samplers};

  vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline->pipelineLayout, 0, 4, dset, 0, NULL

  );

  for (int i = 0; i < array_meshes.len; i++) {
    struct Mesh *mesh = gmArrayGet(&array_meshes, i);

    /*

    Parameters:

            VkCommandBuffer commandBuffer (aka struct VkCommandBuffer_T *)
            uint32_t indexCount (aka unsigned int)
            uint32_t instanceCount (aka unsigned int)
            uint32_t firstIndex (aka unsigned int)
            int32_t vertexOffset (aka int)
            uint32_t firstInstance (aka unsigned int)
    */
    vkCmdDrawIndexed(cmd, mesh->range_index.len, 1, mesh->range_index.start,
                     mesh->range_vertex.start, 0);
  }

  vkCmdEndRendering(cmd);
}

void recordCommandBuffer(uint32_t imageIndex, uint32_t frameIndex) {

  struct Frame *frame = &frames[frameIndex];

  VkCommandBuffer commandBuffer = frame->graphicsCommandBuffers;

  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = 0,
      .pInheritanceInfo = NULL,
  };
  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  {

    VkImageMemoryBarrier2 beginBarrier[] = {

        (VkImageMemoryBarrier2){

            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .srcStageMask = VK_PIPELINE_STAGE_2_NONE,
            .dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,

            .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = swapchain_images[imageIndex].handle,
            .subresourceRange =
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
        },

    };

    VkDependencyInfo beginDepInfo = {

        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = ARR_LEN(beginBarrier),
        .pImageMemoryBarriers = beginBarrier,
    };

    vkCmdPipelineBarrier2(commandBuffer, &beginDepInfo);
  }

  renderMainPass(commandBuffer, frame, imageIndex);

  VkImageMemoryBarrier2 endBarrier[] = {

      (VkImageMemoryBarrier2){
          .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
          .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
          .dstStageMask = VK_PIPELINE_STAGE_2_NONE,
          .srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
          .dstAccessMask = 0,
          .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
          .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
          .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
          .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
          .image = swapchain_images[imageIndex].handle,
          .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                               .baseMipLevel = 0,
                               .levelCount = 1,
                               .baseArrayLayer = 0,
                               .layerCount = 1},
      },

  };

  VkDependencyInfo dependencyInfo = {.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
                                     .imageMemoryBarrierCount =
                                         sizeof(endBarrier) /
                                         sizeof(VkImageMemoryBarrier2),
                                     .pImageMemoryBarriers = endBarrier};
  vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);

  vkEndCommandBuffer(commandBuffer);
}
void updateGameObjects(uint32_t currentFrame) {

  {
    data_globals.time_total = lastTime;
    data_globals.time_delta = deltaTime;
  }
  {

    vec3 cameraCenter;

    glm_vec3_add(cameraPos, cameraFront, cameraCenter);

    glm_lookat(cameraPos, cameraCenter, cameraUp, data_globals.view);
  }
}
void updateBuffers(uint32_t currentFrame) {

  struct Frame *frame = &frames[currentFrame];

  memcpy(frame->ssbo_instances.alloc.mapped, array_data_instances.data,
         sizeof(struct Instance_ssbo) * array_data_instances.len);

  memcpy(frame->ubo_global.alloc.mapped, &data_globals,
         sizeof(struct Global_ubo));

  memcpy(ssbo_materials.alloc.mapped, array_data_materials.data,
         sizeof(struct Material_ssbo) * array_data_materials.len);
}

VkResult acquireNextImage(struct Frame *frame, uint32_t *out_image_index) {

  VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
                                          frame->presentCompleteSemaphore, NULL,
                                          out_image_index);

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    assert(result == VK_TIMEOUT || result == VK_NOT_READY);
    EXIT_CLEAN("failed to acquire swapchain image!");
  }

  return result;
}
void drawFrame() {

  struct Frame *frame = &frames[frameIndex];

  VkResult result =
      vkWaitForFences(device, 1, &frame->inFlightFence, VK_TRUE, UINT64_MAX);

  if (result != VK_SUCCESS) {
    EXIT_CLEAN("failed to wait for fence!");
  }

  uint32_t imageIndex = -1;

  result =
      vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
                            frame->presentCompleteSemaphore, NULL, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {

    recreateSwapChain();

    return;
  }

  updateGameObjects(frameIndex);

  updateBuffers(frameIndex);

  vkResetFences(device, 1, &frame->inFlightFence);

  recordCommandBuffer(imageIndex, frameIndex);

  VkPipelineStageFlags2 stageMask =
      VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo2 submitInfo2 = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,

      .waitSemaphoreInfoCount = 1,
      .pWaitSemaphoreInfos =
          &(VkSemaphoreSubmitInfo){
              .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
              .semaphore = frame->presentCompleteSemaphore,
              .stageMask = stageMask,
              .value = 0,
              .deviceIndex = 0,

          },

      .commandBufferInfoCount = 1,
      .pCommandBufferInfos =
          &(VkCommandBufferSubmitInfo){
              .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
              .commandBuffer = frame->graphicsCommandBuffers,

          },
      .signalSemaphoreInfoCount = 1,
      .pSignalSemaphoreInfos =
          &(VkSemaphoreSubmitInfo){
              .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
              .semaphore = frame->renderFinishedSemaphore,
              .stageMask = stageMask,

          },
  };

  vkQueueSubmit2(graphicsQueue, 1, &submitInfo2, frame->inFlightFence);

  VkPresentInfoKHR presentInfo = {

      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &frame->renderFinishedSemaphore,
      .swapchainCount = 1,
      .pSwapchains = &swapchain,
      .pImageIndices = &imageIndex,

  };

  result = vkQueuePresentKHR(graphicsQueue, &presentInfo);

  if ((result == VK_SUBOPTIMAL_KHR) || (result == VK_ERROR_OUT_OF_DATE_KHR) ||
      framebufferResized) {
    framebufferResized = false;
    recreateSwapChain();
  } else {
    assert(result == VK_SUCCESS);
  }
  frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}
void createSyncObjects() {

  PRINT_FNAME;

  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    struct Frame *frame = &frames[i];

    VkSemaphoreCreateInfo semaphoreCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    vkCreateSemaphore(device, &semaphoreCreateInfo, NULL,
                      &frame->presentCompleteSemaphore);

    vkCreateSemaphore(device, &semaphoreCreateInfo, NULL,
                      &frame->renderFinishedSemaphore);

    VkFenceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    vkCreateFence(device, &createInfo, NULL, &frame->inFlightFence);
  }

  VkFenceCreateInfo fenceInfo = {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO

  };
  vkCreateFence(device, &fenceInfo, NULL, &transferFence);
}

void queueFamilyCheck() {

  PRINT_FNAME;

  uint32_t queueFamilyPropertieCount = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                           &queueFamilyPropertieCount, NULL);

  VkQueueFamilyProperties queueFamilyProperties[queueFamilyPropertieCount];

  vkGetPhysicalDeviceQueueFamilyProperties(
      physicalDevice, &queueFamilyPropertieCount, queueFamilyProperties);

  queueFamilyIndeces[graphicsQueueFamilyArrayIndex] = -1;

  for (int i = 0; i < queueFamilyPropertieCount; i++) {
    printf("queue count: %d, flags: %x\n", queueFamilyProperties[i].queueCount,
           queueFamilyProperties[i].queueFlags);

    if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      queueFamilyIndeces[graphicsQueueFamilyArrayIndex] = i;
      break;
    }
  }
  if (queueFamilyIndeces[graphicsQueueFamilyArrayIndex] == -1) {
    EXIT_CLEAN("No supported VK_QUEUE_GRAPHICS_BIT");
  }

  queueFamilyIndeces[transferQueueFamilyArrayIndex] = -1;

  for (int i = 0; i < queueFamilyPropertieCount; i++) {
    printf("queue count: %d, flags: %x\n", queueFamilyProperties[i].queueCount,
           queueFamilyProperties[i].queueFlags);

    if (queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
      queueFamilyIndeces[transferQueueFamilyArrayIndex] = i;
      break;
    }
  }

  if (queueFamilyIndeces[transferQueueFamilyArrayIndex] == -1) {
    EXIT_CLEAN("No supported VK_QUEUE_TRANSFER_BIT");
  }
}

void createSwapchain() {
  PRINT_FNAME;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface,
                                            &surfaceCapabilities);

  printf("Surface extent: w:%d h:%d\n", surfaceCapabilities.currentExtent.width,
         surfaceCapabilities.currentExtent.height);

  uint32_t physicalDeviceSurfaceFormatCount = 0;

  vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface,
                                       &physicalDeviceSurfaceFormatCount, NULL);

  VkSurfaceFormatKHR surfaceFormats[physicalDeviceSurfaceFormatCount];

  vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface,
                                       &physicalDeviceSurfaceFormatCount,
                                       surfaceFormats);

  printf("Surface formats:\n");

  swapchainSurfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;

  swapchainSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

  for (int i = 0; i < physicalDeviceSurfaceFormatCount; i++) {
    printf("\tSurfaceFormat.format: %d\n", surfaceFormats[i].format);

    if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) {

      swapchainSurfaceFormat = surfaceFormats[i].format;
    }
  }

  for (int i = 0; i < physicalDeviceSurfaceFormatCount; i++) {
    printf("\tSurfaceFormat.colorSpace:%d\n", surfaceFormats[i].colorSpace);

    if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      swapchainSurfaceColorSpace = surfaceFormats[i].colorSpace;
    }
  }

  uint32_t presentModeCount = 0;

  vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                            &presentModeCount, NULL);

  VkPresentModeKHR presentModes[presentModeCount];

  vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                            &presentModeCount, presentModes);

  printf("Surface pPresentModes:\n");

  uint32_t presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

  for (int i = 0; i < presentModeCount; i++) {
    printf("\tpresent mode: %d\n", presentModes[i]);

    if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    }
  }

  swapChainExtent = surfaceCapabilities.currentExtent;

  if (swapChainExtent.width > surfaceCapabilities.maxImageExtent.width &&
      swapChainExtent.height > surfaceCapabilities.maxImageExtent.height) {

    swapChainExtent = (VkExtent2D){
        .width = WIDTH,
        .height = HEIGHT,
    };
  }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  if (width >= surfaceCapabilities.minImageExtent.width &&
      width <= surfaceCapabilities.maxImageExtent.width &&
      height >= surfaceCapabilities.minImageExtent.height &&
      height <= surfaceCapabilities.maxImageExtent.height) {
    swapChainExtent = (VkExtent2D){
        .width = width,
        .height = height,
    };
  }

  /*
          INFO

          Color attachments: at least 4–8 (depends on GPU)
          Depth/stencil: 1

          .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
           - one per queue family
  */

  uint32_t imageCount = 2;

  if (imageCount < surfaceCapabilities.minImageCount) {

    imageCount = surfaceCapabilities.minImageCount;
  }

  if (surfaceCapabilities.maxImageCount > 0 &&
      imageCount > surfaceCapabilities.maxImageCount) {

    imageCount = surfaceCapabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo = {
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .minImageCount = imageCount,
      .imageFormat = swapchainSurfaceFormat,
      .imageColorSpace = swapchainSurfaceColorSpace,
      .imageExtent = swapChainExtent,
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
      .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .preTransform = surfaceCapabilities.currentTransform,
      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .presentMode = presentMode,
      .surface = surface,
      .clipped = true,
  };

  vkCreateSwapchainKHR(device, &createInfo, NULL, &swapchain);
}



// void createShadowPipeline(struct Pipeline *pipeline) {

//   PRINT_FNAME;

//   char *path = pipeline->frag_path;
//   uint8_t *data = NULL;
//   uint32_t dataSize = createShaderFromFile(path, &data);

//   if (dataSize == 0) {

//     printf("Shader path: %s\n", path);
//     EXIT_CLEAN("failed to read fragment shader file\n");
//   }
//   VkShaderModuleCreateInfo createInfo = {
//       .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
//       .pCode = (uint32_t *)data,
//       .codeSize = dataSize,

//   };

//   vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleFrag);

//   free(data);

//   path = pipeline->vert_path;

//   dataSize = createShaderFromFile(path, &data);

//   if (dataSize == 0) {
//     printf("Shader path: %s\n", path);
//     EXIT_CLEAN("failed to read vertext shader file\n");
//   }

//   createInfo = (VkShaderModuleCreateInfo){
//       .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
//       .pCode = (uint32_t *)data,
//       .codeSize = dataSize,

//   };

//   vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleVert);
//   free(data);

//   VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
//       .stage = VK_SHADER_STAGE_VERTEX_BIT,
//       .module = pipeline->shaderModuleVert,
//       .pName = "main",

//   };

//   VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
//       .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
//       .module = pipeline->shaderModuleFrag,
//       .pName = "main",

//   };

//   uint32_t dynamicStateCount = 2;

//   /*
//   INFO
//           set during command buffer recording part
//           vkCmdSetViewport
//   */
//   VkDynamicState dynamicState[] = {VK_DYNAMIC_STATE_VIEWPORT,
//                                    VK_DYNAMIC_STATE_SCISSOR};

//   VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
//       .dynamicStateCount = dynamicStateCount,
//       .pDynamicStates = dynamicState,

//   };

//   VkPipelineShaderStageCreateInfo shaderStageCreateInf[] = {
//       shaderStageCreateInfoVert, shaderStageCreateInfoFrag};

//   uint32_t shaderStageCreateInfCnt = ARR_LEN(shaderStageCreateInf);

//   VkVertexInputBindingDescription vertexInputBindingDescription = {
//       .binding = 0,
//       .stride = sizeof(struct Vertex),
//       .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
//   };

//   VkVertexInputAttributeDescription vertexInputAttributeDescriptions[1];

//   vertexInputAttributeDescriptions[0] = (VkVertexInputAttributeDescription){
//       0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(struct Vertex, pos)};

//   uint32_t vertexInputAttributeDescriptionsCount =
//       sizeof(vertexInputAttributeDescriptions) /
//       (sizeof(VkVertexInputAttributeDescription));

//   VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
//       .vertexBindingDescriptionCount = 1,
//       .pVertexBindingDescriptions = &vertexInputBindingDescription,
//       .vertexAttributeDescriptionCount = vertexInputAttributeDescriptionsCount,
//       .pVertexAttributeDescriptions = vertexInputAttributeDescriptions,

//   };

//   VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo =
//       {
//           .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
//           .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
//       };

//   VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
//       .pViewports = 0,
//       .viewportCount = 1,
//       .pScissors = 0,
//       .scissorCount = 1,
//   };

//   VkPipelineRasterizationStateCreateInfo rasterizer = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
//       .depthClampEnable = VK_FALSE,
//       .rasterizerDiscardEnable = VK_FALSE,
//       .polygonMode = VK_POLYGON_MODE_FILL,
//       .cullMode = VK_CULL_MODE_BACK_BIT,
//       .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
//       .depthBiasEnable = VK_FALSE,
//       .depthBiasSlopeFactor = 1.0f,
//       .lineWidth = 1.0f,

//   };

//   VkPipelineMultisampleStateCreateInfo multisampling = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
//       .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
//       .sampleShadingEnable = VK_FALSE,
//   };

//   VkPipelineDepthStencilStateCreateInfo depthStencil = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
//       .depthTestEnable = VK_TRUE,
//       .depthWriteEnable = VK_TRUE,
//       .depthCompareOp = VK_COMPARE_OP_LESS,
//       .depthBoundsTestEnable = VK_FALSE,
//       .stencilTestEnable = VK_FALSE,
//   };

//   VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
//       .logicOpEnable = VK_FALSE,
//       .logicOp = VK_LOGIC_OP_COPY,
//       .attachmentCount = 0,
//       .pAttachments = NULL,
//   };

//   VkDescriptorSetLayout layouts[] = {
//       descriptor_layout_global, descriptor_layout_instance,
//       descriptor_layout_material, descriptor_layout_sampler};

//   VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
//       .setLayoutCount = ARR_LEN(layouts),
//       .pSetLayouts = layouts,
//       .pushConstantRangeCount = 0,

//   };

//   vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL,
//                          &pipeline->pipelineLayout);

//   VkFormat depthFormat = findDepthFormat();

//   VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
//       .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
//       .colorAttachmentCount = 0,
//       .pColorAttachmentFormats = NULL,
//       .depthAttachmentFormat = depthFormat,
//   };

//   VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
//       .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
//       .pNext = &pipelineRenderingCreateInfo,
//       .stageCount = shaderStageCreateInfCnt,
//       .pStages = shaderStageCreateInf,
//       .pVertexInputState = &pipelineVertexInputStateCreateInfo,
//       .pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo,
//       .pViewportState = &viewportStateCreateInfo,
//       .pRasterizationState = &rasterizer,
//       .pMultisampleState = &multisampling,
//       .pColorBlendState = &colorBlendStateCreateInfo,
//       .pDynamicState = &dynamicStateCreateInfo,
//       .layout = pipeline->pipelineLayout,
//       .renderPass = VK_NULL_HANDLE,
//       .basePipelineHandle = VK_NULL_HANDLE,
//       .basePipelineIndex = -1,
//       .pDepthStencilState = &depthStencil,

//   };

//   vkCreateGraphicsPipelines(device, NULL, 1, &graphicsPipelineCreateInfo, NULL,
//                             &pipeline->graphicsPipeline);
// }

void createPipelines() {
  PRINT_FNAME;

  struct Pipeline *worldPipeline = gmArrayNew(&arrayPipelines);

  worldPipeline->frag_path = "shaders/out/frag.spv";
  worldPipeline->vert_path = "shaders/out/vert.spv";

  createGraphicsPipeline(worldPipeline);
  /*
          create shaders
          create descriptsors
          create pipeline

  */
}

void createGraphicsPipeline(struct Pipeline *pipeline) {

  PRINT_FNAME;

  char *path = pipeline->frag_path;
  uint8_t *data = NULL;
  uint32_t dataSize = createShaderFromFile(path, &data);

  if (dataSize == 0) {

    printf("Shader path: %s\n", path);
    EXIT_CLEAN("failed to read fragment shader file\n");
  }
  VkShaderModuleCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .pCode = (uint32_t *)data,
      .codeSize = dataSize,

  };

  vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleFrag);

  free(data);

  path = pipeline->vert_path;

  dataSize = createShaderFromFile(path, &data);

  if (dataSize == 0) {
    printf("Shader path: %s\n", path);
    EXIT_CLEAN("failed to read vertext shader file\n");
  }

  createInfo = (VkShaderModuleCreateInfo){
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .pCode = (uint32_t *)data,
      .codeSize = dataSize,

  };

  vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleVert);
  free(data);

  VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = pipeline->shaderModuleVert,
      .pName = "main",

  };

  VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = pipeline->shaderModuleFrag,
      .pName = "main",

  };

  uint32_t dynamicStateCount = 2;

  /*
  INFO
          set during command buffer recording part
          vkCmdSetViewport
  */
  VkDynamicState dynamicState[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                   VK_DYNAMIC_STATE_SCISSOR};

  VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .dynamicStateCount = dynamicStateCount,
      .pDynamicStates = dynamicState,

  };

  VkPipelineShaderStageCreateInfo shaderStageCreateInf[] = {
      shaderStageCreateInfoVert, shaderStageCreateInfoFrag};

  uint32_t shaderStageCreateInfCnt = ARR_LEN(shaderStageCreateInf);

  VkVertexInputBindingDescription vertexInputBindingDescription = {
      .binding = 0,
      .stride = sizeof(struct Vertex),
      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
  };

  VkVertexInputAttributeDescription vertexInputAttributeDescriptions[3];

  vertexInputAttributeDescriptions[0] = (VkVertexInputAttributeDescription){
      0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(struct Vertex, pos)};
  vertexInputAttributeDescriptions[1] = (VkVertexInputAttributeDescription){
      1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(struct Vertex, norm)};
  vertexInputAttributeDescriptions[2] = (VkVertexInputAttributeDescription){
      2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(struct Vertex, texCoords)};

  uint32_t vertexInputAttributeDescriptionsCount =
      sizeof(vertexInputAttributeDescriptions) /
      (sizeof(VkVertexInputAttributeDescription));

  VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &vertexInputBindingDescription,
      .vertexAttributeDescriptionCount = vertexInputAttributeDescriptionsCount,
      .pVertexAttributeDescriptions = vertexInputAttributeDescriptions,

  };

  VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo =
      {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
          .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      };

  VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .pViewports = 0,
      .viewportCount = 1,
      .pScissors = 0,
      .scissorCount = 1,
  };

  VkPipelineRasterizationStateCreateInfo rasterizer = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .depthClampEnable = VK_FALSE,
      .rasterizerDiscardEnable = VK_FALSE,
      .polygonMode = VK_POLYGON_MODE_FILL,
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
      .depthBiasEnable = VK_FALSE,
      .depthBiasSlopeFactor = 1.0f,
      .lineWidth = 1.0f,

  };

  VkPipelineMultisampleStateCreateInfo multisampling = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .sampleShadingEnable = VK_FALSE,
  };

  VkPipelineDepthStencilStateCreateInfo depthStencil = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
      .depthTestEnable = VK_TRUE,
      .depthWriteEnable = VK_TRUE,
      .depthCompareOp = VK_COMPARE_OP_LESS,
      .depthBoundsTestEnable = VK_FALSE,
      .stencilTestEnable = VK_FALSE,
  };

  VkPipelineColorBlendAttachmentState colorBlendAttachmentState[] = {

      (VkPipelineColorBlendAttachmentState){
          .blendEnable = VK_FALSE,

          .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                            VK_COLOR_COMPONENT_G_BIT |
                            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
          .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
          .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
          .colorBlendOp = VK_BLEND_OP_ADD,
          .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
          .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
          .alphaBlendOp = VK_BLEND_OP_ADD,
      },

  };

  VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY,
      .attachmentCount = sizeof(colorBlendAttachmentState) /
                         sizeof(colorBlendAttachmentState[0]),
      .pAttachments = colorBlendAttachmentState,
  };

  VkDescriptorSetLayout layouts[] = {
      descriptor_layout_global, descriptor_layout_instance,
      descriptor_layout_material, descriptor_layout_sampler};

  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .setLayoutCount = ARR_LEN(layouts),
      .pSetLayouts = layouts,
      .pushConstantRangeCount = 0,
      .pPushConstantRanges = NULL};

  vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL,
                         &pipeline->pipelineLayout);

  VkFormat depthFormat = findDepthFormat();

  VkFormat formats[] = {
      swapchainSurfaceFormat,

  };

  VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
      .colorAttachmentCount = ARR_LEN(formats),
      .pColorAttachmentFormats = formats,
      .depthAttachmentFormat = depthFormat,
  };

  VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .pNext = &pipelineRenderingCreateInfo,
      .stageCount = shaderStageCreateInfCnt,
      .pStages = shaderStageCreateInf,
      .pVertexInputState = &pipelineVertexInputStateCreateInfo,
      .pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo,
      .pViewportState = &viewportStateCreateInfo,
      .pRasterizationState = &rasterizer,
      .pMultisampleState = &multisampling,
      .pColorBlendState = &colorBlendStateCreateInfo,
      .pDynamicState = &dynamicStateCreateInfo,
      .layout = pipeline->pipelineLayout,
      .renderPass = VK_NULL_HANDLE,
      .basePipelineHandle = VK_NULL_HANDLE,
      .basePipelineIndex = -1,
      .pDepthStencilState = &depthStencil,

  };

  vkCreateGraphicsPipelines(device, NULL, 1, &graphicsPipelineCreateInfo, NULL,
                            &pipeline->graphicsPipeline);
}

void initVulkan() {
  PRINT_FNAME;

  createInstance();

  setupDebugMessenger();

  // createSurface();

  createPhysicalDevice();

  queueFamilyCheck();

  physicalDeviceExtensionCheck();

  physicalDeviceFeatureCheck();

  createLogicalDevice();

  createSwapchain();

  createImageViews();

  createQueue();

  createDescriptors();

  createPipelines();

  createCommandPool();

  createSyncObjects();

  createResources();

  updateDefaultDescriptors();

  createDepthImages();

  initMeshes();

  createMeshBuffers();
}
void mainLoop() {
  PRINT_FNAME;

  startTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    processInput(window);

    drawFrame();
  }

  vkDeviceWaitIdle(device);
}

void cleanup() {


  cleanupSwapChain();
  // vkDestroySwapchainKHR(device, swapchain,NULL);

  // for(int i=0;i < swapchain_img_cnt;i++){

  // 	// cleanImageRes(&swapchain_images[i]);
  // 	vkDestroyImageView(device, swapchain_images[i].view, NULL);
  // }

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    struct Frame *frame = &frames[i];

    vkDestroySemaphore(device, frame->presentCompleteSemaphore, NULL);

    vkDestroySemaphore(device, frame->renderFinishedSemaphore, NULL);

    vkDestroyFence(device, frame->inFlightFence, NULL);
  }

  // cleanupPickImages();

  vkDestroyFence(device, transferFence, NULL);

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    struct Frame *frame = &frames[i];

    cleanImageRes(&frame->deapth);
  }

  cleanTextureRes(&tex_res0);

  for (int i = 0; i < arrayPipelines.len; i++) {
    struct Pipeline *pipeline = gmArrayGet(&arrayPipelines, i);

    vkDestroyPipeline(device, pipeline->graphicsPipeline, NULL);

    vkDestroyPipelineLayout(device, pipeline->pipelineLayout, NULL);

    vkDestroyShaderModule(device, pipeline->shaderModuleFrag, NULL);

    vkDestroyShaderModule(device, pipeline->shaderModuleVert, NULL);
  }

  vkDestroyDescriptorSetLayout(device, descriptor_layout_global, NULL);
  vkDestroyDescriptorSetLayout(device, descriptor_layout_instance, NULL);
  vkDestroyDescriptorSetLayout(device, descriptor_layout_material, NULL);
  vkDestroyDescriptorSetLayout(device, descriptor_layout_sampler, NULL);

  vkFreeDescriptorSets(device, descriptor_pool, 1, &descriptor_set_materials);
  vkFreeDescriptorSets(device, descriptor_pool, MAX_FRAMES_IN_FLIGHT,
                       descriptor_set_instances);
  vkFreeDescriptorSets(device, descriptor_pool, MAX_FRAMES_IN_FLIGHT,
                       descriptor_set_globals);
  vkFreeDescriptorSets(device, descriptor_pool, 1, &descriptor_set_samplers);

  vkDestroyDescriptorPool(device, descriptor_pool, NULL);

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

    vkFreeCommandBuffers(device, graphicsCommnadPool, 1,
                         &frames[i].graphicsCommandBuffers);
  }

  vkFreeCommandBuffers(device, transferCommnadPool, 1, &transferCommandBuffers);

  vkDestroyCommandPool(device, graphicsCommnadPool, NULL);

  vkDestroyCommandPool(device, transferCommnadPool, NULL);

  cleanBuffer(&vertex_buffer);
  cleanBuffer(&index_buffer);

  cleanShaderBuffers();

  vkDestroyDevice(device, NULL);

  vkDestroySurfaceKHR(instance, surface, NULL);

  vkDestroyInstance(instance, NULL);

  glfwDestroyWindow(window);

  glfwTerminate();
}

int main() {

  PRINT_FNAME;

  initVariables();

  initWindow();
  initVulkan();

  initGameObjects();

  mainLoop();
  cleanup();

  freeVariables();

  return 0;
};
