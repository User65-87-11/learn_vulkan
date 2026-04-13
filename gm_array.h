#pragma once

#include <stdint.h>

struct GmArray{
	uint32_t len;
	uint32_t capacity;
	uint32_t width;
	
};

/**
partiition array into  views?

*/

size_t gmArrayLength(void * src);

void gmArrayClear(void * src);

void gmArrayCreate(void ** dst, uint32_t width, uint32_t size);

void gmArrayViewCreate(void ** dst,void * src, uint32_t size);

void gmArrayViewFree(void ** dst);

void gmArrayAdd(void ** dst, void * element);

void gmArraySwap(void *dst, uint32_t pos1, uint32_t pos2);

void gmArrayRemove(void *dst, uint32_t position);

void gmArrayGet(void * src, uint32_t idx, void* dst);

void gmArraySet(void * dst, uint32_t idx, void* src);

void gmArrayFree(void ** dst);
