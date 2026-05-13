#include "shader.h"
#include <time.h>
#include <vulkan/vulkan_core.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "device.h"
#include "util/common.h"

#ifdef _WIN32
#include <direct.h>
#include <stdlib.h>
#else
#include <unistd.h>

#endif

static uint32_t readFile(const char* path, uint8_t** buffer);

void Shader_Destroy(VkDevice device, VkShaderModule shader) {
	vkDestroyShaderModule(device, shader, NULL);
}

VkShaderModule Shader_CreateFromFile(VkDevice device, const char* path) {

	VkShaderModule retval;
	uint8_t* data = NULL;
	uint32_t dataSize = readFile(path, &data);

	if (dataSize == 0) {

		printf("Shader path: %s\n", path);
		EXIT_CLEAN("failed to read fragment shader file\n");
	}
	VkShaderModuleCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,

	};

	vkCreateShaderModule(device, &createInfo, NULL, &retval);

	free(data);

	return retval;
};

static uint32_t readFile(const char* path, uint8_t** buffer) {

	PRINT_FNAME;

	char cwd[256];

	if (_getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working directory:\n");
		printf("%s\n", cwd);
		printf("%s\n", path);
	} else {
		perror("_getcwd() error");
	}

	FILE* file = NULL;

	errno_t err = fopen_s(&file, path, "rb");

	// FILE *file = fopen_s(path, "rb");

	if (err != 0 || file == NULL) {
		printf("Error opening file %s\n", file);
		return 1;
	}

	fseek(file, 0, SEEK_END);
	uint32_t size = ftell(file);
	rewind(file);

	*buffer = malloc(size);
	if (*buffer == NULL) {
		printf("Fill buffer mem alloc error\n");
		fclose(file);
		return 0;
	}

	fread(*buffer, 1, size, file);

	fclose(file);

	return size;
}
