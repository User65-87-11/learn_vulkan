#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "gm_array.h"

#ifdef _WIN32
#include <malloc.h>
#define gm_alloc_aligned(size, align) _aligned_malloc(size, align)
#define gm_free_aligned(ptr) _aligned_free(ptr)
#else
#include <stdlib.h>
#define gm_alloc_aligned(size, align) aligned_alloc(align, size)
#define gm_free_aligned(ptr) free(ptr)
#endif

static inline uint32_t align_up(uint32_t x, uint32_t a) {
  return (x + (a - 1)) & ~(a - 1);
}

void gmArrayInit(struct GmArray *array, uint32_t width, uint32_t initial_size,
                 uint32_t alignment) {
  assert(array->data == NULL);

  array->len = 0;
  array->capacity = initial_size;
  array->width = align_up(width, alignment);
  array->data = NULL;
  array->alignment = alignment;
  // array->stride = align_up(width, DEFAULT_ALIGN);
  if (initial_size != 0 && array->width != 0) {
    array->data = gm_alloc_aligned(array->width * initial_size, alignment);
    memset(array->data, 0, array->width * initial_size);
    // array->data = malloc(array->width * initial_size);
  }
}

void *gmArrayNew(struct GmArray *array) {

  assert(array->data != NULL);

  if (array->len >= array->capacity) {

    uint32_t old_size = array->len * array->width;

    if (array->capacity == 0) {

      array->capacity = 4;
    } else {
      array->capacity *= 2;
    }

    uint32_t new_size = array->capacity * array->width;

    void *new_data = gm_alloc_aligned(new_size, array->alignment);

    assert(new_data != NULL);

    memcpy(new_data, array->data, old_size);

    memset((char *)new_data + old_size, 0, new_size - old_size);

    gm_free_aligned(array->data);

    array->data = new_data;

    // array->capacity*=2;
    // realloc(array->data,array->capacity * array->width);
  }

  void *slot = (char *)array->data + array->len * array->width;
  memset(slot, 0, array->width);
  array->len++;

  return slot;
}

void *gmArrayNewN(struct GmArray *array, uint32_t num) {
  assert(array->data != NULL);

  if (array->len + num > array->capacity) {

    uint32_t old_size = array->len * array->width;

    if (array->capacity == 0) {

      array->capacity = 4;
    }

    while (array->len + num > array->capacity) {
      array->capacity *= 2;
    }

    uint32_t new_size = array->capacity * array->width;

    void *new_data = gm_alloc_aligned(new_size, array->alignment);

    assert(new_data != NULL);

    memcpy(new_data, array->data, old_size);

    memset((char *)new_data + old_size, 0, new_size - old_size);

    gm_free_aligned(array->data);
    array->data = new_data;

    // array->capacity*=2;
    // realloc(array->data,array->capacity * array->width);
  }

  void *slot = (char *)array->data + array->len * array->width;
  memset(slot, 0, num * array->width);
  array->len += num;

  return slot;
};
// void gmArrayCopyBySize(void * dst, struct GmArray * array){

// 	uint8_t *src_base = (uint8_t *)array->data;
//     uint8_t *dst_base = (uint8_t *)dst;
// 	uint32_t size =  array->width;

//     for (uint32_t i = 0; i < array->len; i++) {
//         memcpy(
//             dst_base + i * size,
//             src_base + i * array->stride,
//             size
//         );
//     }
// }
void gmArrayPushValue(struct GmArray *array, void *elem) {

  assert(array->data != NULL);

  if (array->len >= array->capacity) {
    uint32_t old_size = array->capacity * array->width;

    array->capacity *= 2;

    uint32_t new_size = array->capacity * array->width;

    void *new_data = gm_alloc_aligned(new_size, array->alignment);
    memcpy(new_data, array->data, old_size);
    gm_free_aligned(array->data);
    array->data = new_data;

    // array->data = realloc(array->data, array->capacity * array->width);

    // array->capacity *= 2;

    // realloc(array->data,array->capacity * array->width);
  }

  memcpy((char *)array->data + array->len * array->width, elem, array->width);

  array->len++;
}

void *gmArrayGet(struct GmArray *array, uint32_t position) {
  assert(array->data != NULL && position < array->len);

  return (char *)array->data + position * array->width;
}

void gmArrayFree(struct GmArray *array) {
  assert(array->data != NULL);
  gm_free_aligned(array->data);
  array->data = NULL;
  array->len = 0;
}

void gmArrayRemove(struct GmArray *array, uint32_t position) {

  void *base = array->data;

  void *target = (char *)base + position * array->width;
  void *last = (char *)base + (array->len - 1) * array->width;

  memcpy(target, last, array->width);
  array->len--;
}

void gmArrayViewCreateSlice(struct GmArray *array,
                            struct GmArrayView *arrayView, uint32_t start,
                            uint32_t length) {

  assert(array->data != NULL && start < array->len &&
         (start + length) <= array->len);

  arrayView->data = (char *)array->data + start * array->width;
  arrayView->width = array->width;
  arrayView->len = length;
  arrayView->offset = start;
}

// void* gmArrayViewGet( struct GmArrayView *arrayView, uint32_t idx){
// 	assert(idx < arrayView->len);

// 	return &arrayView->data[idx];
// }

void gmArrayViewRemove(struct GmArrayView *arrayView, uint32_t position) {
  assert(position < arrayView->len);

  void *base = arrayView->data;

  void *target = (char *)base + position * arrayView->width;
  void *last = (char *)base + (arrayView->len - 1) * arrayView->width;

  memcpy(target, last, arrayView->width);
  arrayView->len--;
}
