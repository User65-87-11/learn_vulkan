#include "instance.h"
#include "platform.h"
#include "util/common.h"
#include <GLFW/glfw3.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

static VkInstance instance = NULL;

static VkDebugUtilsMessengerEXT debugUtilsMessengerEXT = NULL;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

static uint32_t validationLayerCnt = 1;
static const char* validationLayers[] = {
	"VK_LAYER_KHRONOS_validation",
};

static VkBool32 debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	printf("[validation layer]: %d %d %s\n", messageSeverity, messageTypes,
		pCallbackData->pMessage);
	return VK_FALSE;
}

static void setupDebugMessenger() {
	PRINT_FNAME;

	if (!enableValidationLayers)
		return;

	VkDebugUtilsMessageSeverityFlagsEXT severityFlags =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	VkDebugUtilsMessageTypeFlagsEXT messageTypeFlags =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = severityFlags,
		.messageType = messageTypeFlags,
		.pfnUserCallback = debugCallback};
	assert(instance);

	PFN_vkCreateDebugUtilsMessengerEXT func =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != NULL) {
		if (func(instance, &debugUtilsMessengerCreateInfoEXT, NULL,
				&debugUtilsMessengerEXT) != VK_SUCCESS) {
			printf("%s\n", "cannot setup debug messenger");
		}

	} else {

		printf("[NULl] %s\n", "PFN_vkCreateDebugUtilsMessengerEXT");
	}
}

void Instance_Create() {
	PRINT_FNAME;

	VkApplicationInfo applicationInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_4};

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions =
		glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	printf("glfwExtensionCount %d\n", glfwExtensionCount);

	uint32_t glfwExtensionCountExtra = glfwExtensionCount + 1;

	const char* glfwExtensionsExtra[glfwExtensionCountExtra];

	for (int i = 0; i < glfwExtensionCount; i++) {
		glfwExtensionsExtra[i] = glfwExtensions[i];
	}

	if (enableValidationLayers) {

		glfwExtensionsExtra[glfwExtensionCount] =
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
	} else {
		glfwExtensionCountExtra = glfwExtensionCount;
	}

	for (int i = 0; i < glfwExtensionCountExtra; i++) {

		printf("\tglfw required extensions: %s\n", glfwExtensionsExtra[i]);
	}

	{

		uint32_t layerCount = 0;
		VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, NULL);
		if (result != VK_SUCCESS) {
			printf("Failed to get layer count\n");
			return;
		}

		VkLayerProperties layers[layerCount];
		result = vkEnumerateInstanceLayerProperties(&layerCount, layers);
		if (result != VK_SUCCESS) {
			printf("Failed to enumerate layers\n");

			return;
		}

		printf("Print layers: %d\n", layerCount);
		bool validationLayerSupported = false;
		for (uint32_t i = 0; i < layerCount; i++) {

			printf("layer: %s\n", layers[i].layerName);

			uint32_t cnt = 0;
			vkEnumerateInstanceExtensionProperties(
				layers[i].layerName, &cnt, NULL);

			if (cnt > 0) {
				VkExtensionProperties expr[cnt];
				vkEnumerateInstanceExtensionProperties(
					layers[i].layerName, &cnt, expr);

				for (int i = 0; i < cnt; i++) {

					printf("\tlayer extensions: %s\n", expr[i].extensionName);
				}
			}

			if (strcmp(layers[i].layerName, validationLayers[0]) == 0) {
				validationLayerSupported = true;
			}
		}
		if (validationLayerSupported == false) {
			printf("Required layer is not supported \n\t%s\n",
				validationLayers[0]);
			return;
		}
	}

	VkInstanceCreateInfo instanceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &applicationInfo,
		.enabledExtensionCount = glfwExtensionCountExtra,
		.ppEnabledExtensionNames = glfwExtensionsExtra,

	};

	if (enableValidationLayers) {
		instanceCreateInfo.enabledLayerCount = validationLayerCnt;
		instanceCreateInfo.ppEnabledLayerNames = validationLayers;
	}

	if (vkCreateInstance(&instanceCreateInfo, NULL, &instance) != VK_SUCCESS) {

		EXIT_CLEAN("vkCreateInstance failed");
	} else {
		printf("vkCreateInstance CREATED\n");
	}

	setupDebugMessenger();
}

void Instance_DestroyInstance() {

	if (enableValidationLayers) {
		PFN_vkDestroyDebugUtilsMessengerEXT func =
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
				instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != NULL) {
			func(instance, debugUtilsMessengerEXT, NULL);
		}
	}
}

VkInstance Instance_Get() { return instance; }
