#include "GLFW/glfw3.h"
#include "app.h"
#include "assets/asset0.h"
#include "descriptor.h"
#include "device2.h"
#include "input.h"
#include "instance.h"
#include "platform.h"
#include "renderer.h"
#include "scene.h"
#include "shader_common.h"
#include "util/common.h"
#include <stdlib.h>

struct ApplicationContext app_info;

static void mainLoop();

static void callback_FrameBuffer_Resize(void * window,uint32_t width,uint32_t height);


void App_start() { mainLoop(); }

static void mainLoop() {
	PRINT_FNAME;

	printf("struct InstanceData align on %d\n", _Alignof(struct InstanceData));
	printf("struct InstanceData sizeof on %d\n", sizeof(struct InstanceData));

	printf("struct Camera _Alignof on %d\n", _Alignof(struct CameraData));
	printf("struct Camera sizeof on %d\n", sizeof(struct CameraData));
	printf("struct Camera sizeof % 16 %d\n", sizeof(struct CameraData) % 16);
	if (sizeof(struct CameraData) % 32 != 0) {

		printf("struct is missing %d\n", sizeof(struct CameraData) % 32);
		exit(1);
	}

	// void (* callbacks[])(void * win,uint32_t w,uint32_t h)={
	// 	Renderer_callback_FrameBuffer_Resize,
	// 	Scene_callback_FrameBuffer_Resize
	// };

	// app_info.callbacks_cnt = 2;
	// app_info.callbacks = malloc(sizeof(struct Platform_callback)* app_info.callbacks_cnt);
	app_info.callbacks[0].callback_resize = Scene_callback_FrameBuffer_Resize;
	app_info.callbacks[0].dst = &app_info.scene;

	app_info.callbacks[1].callback_resize = Renderer_callback_FrameBuffer_Resize;
	app_info.callbacks[1].dst = &app_info.renderer;
	

	
	struct Plaftorm_info platform_info = {
		.ref_inst = &app_info.instance,
		.callback_resize = app_info.callbacks,
		.collback_cnt = ARR_LEN(app_info.callbacks),
		.width = WIDTH,
		.height = HEIGHT,
		.cursor_state = GLFW_CURSOR_DISABLED
	};


		
	Platform_Init(&platform_info,&app_info.platform);
	


	const char* layers[] = {"VK_LAYER_KHRONOS_validation"};
	struct Instance_Into instance_info={
		.enableValidationLayers = true,
		.validationLayers = layers,
		.validationLayer_cnt = ARR_LEN(layers),
	};

	// Instance_CreateInstance(&instance_info, &app_info->instance);

	Instance_Create(&instance_info, &app_info.instance);



	Platform_createSurface(&app_info.platform, &app_info.platform.surface);


	struct Input_Backend * input_backend = &app_info.input_backend;
	{
		input_backend->window = app_info.platform.window;
		input_backend->get_key =  Platform_GetKey;
		input_backend->get_mouse_button =  Platform_GetMouseButton;
		input_backend->get_mouse_pos =  Platform_GetMousePosition;
		
	}
	
	
	Input_Init(&app_info.input);
	

	
	const char * required_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};
	struct Device_CreateInfo device_info = {
		.requiredDeviceExtensions = required_extensions,
		.requiredDeviceExtensions_cnt = ARR_LEN(required_extensions),
		.ref_instance = &app_info.instance,
		.ref_platform = &app_info.platform,
	};
	
	Device_Create(&device_info, &app_info.device);

	


	struct Descriptor_InitInfo descriptor_info={
		.device_ref = &app_info.device
	};
	Descriptor_Init(&descriptor_info,&app_info.descriptor);

	uint32_t width,height;
 	Platform_GetFramebufferSize(&app_info.platform,&width,&height);

	struct Scene_Info scene_info = {
		.ref_device = &app_info.device, 
		.ref_input = &app_info.input,
		.ref_platform = &app_info.platform,
		.width = width,
		.height = height
	};

	Scene_Init(&scene_info,&app_info.scene);

	struct Renderer_info   renderer_info = {
		.ref_device = &app_info.device,
		.ref_instance = &app_info.instance,
		.ref_descriptor = &app_info.descriptor,
		.ref_platform = &app_info.platform,
		.ref_scene = &app_info.scene
	};
	
	Renderer_Init(&renderer_info,&app_info.renderer);


	Asset0_init(&app_info);

	int running = 1;

	float lastTime = Platform_GetTime(&app_info.platform);

	while (running) {

		double currentTime = Platform_GetTime(&app_info.platform);
		float dt = (float)(currentTime - lastTime);
		lastTime = currentTime;

		Platform_PollEvents(&app_info.platform);

		Input_Update(&app_info.input,&app_info.input_backend);

		Scene_Update(&app_info.scene, dt);

		Renderer_Render(
			&app_info.renderer, currentTime, dt);

		if (Platform_ShouldCloseWindow(&app_info.platform)) {
			running = 0;
		}
	}

	Swapchain_Destroy(&app_info.renderer.swapchain);

	Renderer_Destroy(&app_info.renderer);

	Descriptor_Destroy(&app_info.descriptor);

	Device_Destroy(&app_info.device);

	Platform_destroySurface(&app_info.platform,app_info.platform.surface);

	Platform_Shutdown(&app_info.platform);

	Instance_DestroyInstance(&app_info.instance);


	
}

void App_destroy(struct ApplicationContext * app){
	
}
