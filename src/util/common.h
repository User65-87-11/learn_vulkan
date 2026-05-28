#pragma once
#include <stdio.h>

#include "cglm/cglm.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/util.h"



#define MAX_IMAGE_VIEWS 16

#define MAX_FRAMES_IN_FLIGHT 2

#define MAX_VERTICES  100000

#define MAX_INDICES   300000


// extern void cleanup();
#define PRINT_FNAME printf("Call to: %s\n", __FUNCTION__)

#define ARR_LEN(A) sizeof(A) / sizeof(A[0])

#define EXIT_CLEAN(msg)                                                        \
  do {                                                                         \
    printf("ERROR: %s\n", msg);                                                \
    exit(1);                                                                   \
  } while (0)


  
#define EXIT_PRINT(msg)                                                        \
do {                                                                         \
    printf("ERROR: %s\n", msg);                                                \
    exit(1);                                                                   \
} while (0)

    
#define GLM_VEC3_COPY(dst, src)                                                \
  do {                                                                         \
    dst[0] = src[0];                                                           \
    dst[1] = src[1];                                                           \
    dst[2] = src[2];                                                           \
  } while (0)

#define GLM_MAT4_COPY(dst, src) memcpy(dst, src, sizeof(mat4))

#define GLM_VEC4_COPY(dst, src)                                                \
  do {                                                                         \
    dst[0] = src[0];                                                           \
    dst[1] = src[1];                                                           \
    dst[2] = src[2];                                                           \
    dst[3] = src[3];                                                           \
  } while (0)

#define GLM_VEC4_SET(dst, a, b, c, d)                                          \
  do {                                                                         \
    dst[0] = a;                                                                \
    dst[1] = b;                                                                \
    dst[2] = c;                                                                \
    dst[3] = d;                                                                \
  } while (0)

#define GLM_VEC3_SET(dst, a, b, c)                                             \
  do {                                                                         \
    dst[0] = a;                                                                \
    dst[1] = b;                                                                \
    dst[2] = c;                                                                \
  } while (0)


  struct Range {
    uint32_t start;
    uint32_t len;
  };


  #define VK_CHECK(api)                                                      \
  do {                                                                        \
      VkResult result = (api);                                                \
      if (result != VK_SUCCESS) {                                             \
          fprintf(stderr,                                                     \
                  "Vulkan call failed\n"                                      \
                  "Call : %s\n"                                               \
                  "Error: %d\n"                                               \
                  "File : %s\n"                                               \
                  "Line : %d\n"                                               \
                  "Func : %s\n",                                              \
                  #api,                                                       \
                  result,                                                     \
                  __FILE__,                                                   \
                  __LINE__,                                                   \
                  __func__);                                                  \
          exit(1);                                                            \
      }                                                                       \
  } while (0)
