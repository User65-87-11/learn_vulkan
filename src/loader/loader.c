#include <stdint.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "../util/common.h"
#include "loader.h"
#include "gltf_asset.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

static void print_gltf(char* path);

void Loader_LoadImageDataFromFile(char* path, struct ImageData* out) {

	int w, h, c;

	stbi_uc* pixels = stbi_load(path, &w, &h, &c, STBI_rgb_alpha);

	if (!pixels) {
		EXIT_CLEAN("failed to load texture image!");
	}
	out->width = w;
	out->height = h;
	out->channels = c;
	out->data = pixels;
	out->size = w * h * 4;
}
void Loader_LoadImageDataFromMemory(void* data, uint32_t size,
									struct ImageData* out) {

	// int texWidth, texHeight, texChannels;
	int w, h, c;
	stbi_uc* pixels =
		stbi_load_from_memory(data, size, &w, &h, &c, STBI_rgb_alpha);

	if (!pixels) {
		EXIT_CLEAN("failed to load texture image!");
	}

	out->width = w;
	out->height = h;
	out->channels = c;
	out->data = pixels;
	out->size = size;
}
void Loader_FreeImageData(struct ImageData* image_data) {
	if (image_data->data != NULL) {
		stbi_image_free(image_data->data);
		image_data->data = NULL;
	}


}
