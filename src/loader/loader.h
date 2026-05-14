#pragma once
#include <stdint.h>

#include "gltf_asset.h"

struct ImageData {
	uint32_t width;
	uint32_t height;
	uint32_t channels;
	uint32_t color_num;
	void* data;
	uint32_t size;
};

void Loader_LoadImageDataFromFile(char* path, struct ImageData* out);

void Loader_LoadImageDataFromMemory(
	void* data, 
	uint32_t size, 
	struct ImageData* out
);

void Loader_FreeImageData(struct ImageData* out);

void Loader_LoadGLTF(
    char* path, 
    struct GltfAsset* asset
);
