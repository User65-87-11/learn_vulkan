#pragma once
#include <vulkan/vulkan_core.h>

struct Instance_Into{
	const char** validationLayers;
	uint32_t validationLayer_cnt;
	bool enableValidationLayers;
	
};

struct Instance_State{
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugUtilsMessengerEXT ;
	bool enableValidationLayers ;
};

void Instance_Create(struct Instance_Into *info,struct Instance_State * instance);

void Instance_DestroyInstance(struct Instance_State * instance);

// VkInstance Instance_Get();
