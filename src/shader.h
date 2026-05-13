#pragma once
#include <vulkan/vulkan_core.h>

VkShaderModule Shader_CreateFromMemory(
	VkDevice device, const void* data, size_t size);
VkShaderModule Shader_CreateFromFile(VkDevice device, const char* path);

void Shader_Destroy(VkDevice device, VkShaderModule shader);
