#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

 
#include "gm_array.h"

static inline struct GmArray *gmArrayHeader(void *arr) {
    return ((struct GmArray *)arr) - 1;
}


void gmArrayCreate(void ** dst, uint32_t width, uint32_t size){
	assert(*dst == NULL);
	assert(width !=0);


	
    struct GmArray *header =
        malloc(sizeof(struct GmArray) + width * size);

    header->len = 0;
    header->capacity = size;
    header->width = width;

    // return pointer to usable data
    *dst = (void *)(header + 1);

	// *a- = width;
	// *a->len = size;
	// if(size < 256){
	// 	size = 256;
	// }
	// a->capacity = size;
	 

}
void gmArrayAdd(void **dst, void *element) {
   assert(dst && *dst && element);

    struct GmArray *h = gmArrayHeader(*dst);

    // grow if needed
    if (h->len == h->capacity) {
        uint32_t new_capacity = h->capacity ? h->capacity * 2 : 1;

        h = realloc(h, sizeof(*h) + h->width * new_capacity);
        assert(h);

        h->capacity = new_capacity;

        *dst = (void *)(h + 1);
    }


    void *target = (char *)(*dst) + h->len * h->width;
    memcpy(target, element, h->width);

    h->len++;
}
void gmArraySet(void *dst, uint32_t idx, void *src_elem) {
    assert(dst && src_elem);

    struct GmArray *h = gmArrayHeader(dst);
    assert(idx < h->len);

    void *target = (char *)(dst) + idx * h->width;
    memcpy(target, src_elem, h->width);
}
size_t gmArrayLength(void * src){
 	assert(src);
	struct GmArray *h = gmArrayHeader(src);

	return h->len;
}
void gmArrayClear(void * src){
	assert(src);
	struct GmArray * h = gmArrayHeader(src);
	h->len = 0;

}
void gmArrayGet(void *src, uint32_t idx, void *dst) {
    assert(src && dst);

    struct GmArray *h = gmArrayHeader(src);
    assert(idx < h->len);

    void *source = (char *)(src) + idx * h->width;
    memcpy(dst, source, h->width);
}
void gmArrayFree(void **dst) {
    if (!dst || !*dst) return;

    struct GmArray *h = gmArrayHeader(*dst);
    free(h);

    *dst = NULL;
}