#pragma once

#include <stdint.h>

struct GmArray{
	uint32_t len;
	uint32_t capacity;
	uint32_t width;

};



size_t gmArrayLength(void * src);

void gmArrayClear(void * src);

void gmArrayCreate(void ** dst, uint32_t width, uint32_t size);

void gmArrayAdd(void ** dst, void * element);

void gmArrayGet(void * src, uint32_t idx, void* dst);

void gmArraySet(void * dst, uint32_t idx, void* src);

void gmArrayFree(void ** dst);
