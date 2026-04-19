#pragma once

#include <stdint.h>

struct GmArray{
	void * data;
	uint32_t len;
	uint32_t width;
	uint32_t stride;
	uint32_t capacity;
	uint32_t alignment;

	
};

struct GmArrayView{
	void * data;
	uint32_t len;
	uint32_t width;
	// uint32_t stride;
	uint32_t offset;
	
};


/**
partiition array into  views?

*/

void gmArrayInit(struct GmArray * array, uint32_t width, uint32_t initial_size, uint32_t alignment);

// void gmArrayCopyTo(void * dst, struct GmArray * array);

void* gmArrayPush( struct GmArray * array);

void gmArrayPushValue(struct GmArray * array, void * elem);



void* gmArrayGet(struct GmArray * array, uint32_t position);



void gmArrayFree(struct GmArray * array);

void gmArrayRemove(struct GmArray *array, uint32_t position);

void gmArrayViewCreateSlice(struct GmArray * src, struct GmArrayView * dst, uint32_t start, uint32_t length);

// void* gmArrayViewGet( struct GmArrayView *src,uint32_t idx);

void gmArrayViewRemove( struct GmArrayView *array, uint32_t position);

/*remove by swapping with the last element*/


