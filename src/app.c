#include "GLFW/glfw3.h"


#include "app.h"
#include "assets/asset0.h"
#include "descriptor.h"
#include "device.h"
#include "input.h"
#include "instance.h"
#include "mess.h"
#include "platform.h"
// #include "renderer.h"
// #include "scene.h"
#include "shader/common.h"
#include "common.h"


struct ApplicationContext app_info;

static void mainLoop();




void App_start() { mainLoop(); }

static void mainLoop() {
	PRINT_FNAME;



	// printf("struct Scene_Data align on %d\n", _Alignof(struct Scene_Data));
	// printf("struct Scene_Data sizeof on %d\n", sizeof(struct Scene_Data));

	// printf("  Scene_Data.global_data offsetof on %d\n", offsetof(struct Scene_Data,global_data));
	// printf("  Scene_Data.camera_data offsetof on %d\n", offsetof(struct Scene_Data,camera_data));

	// printf("  Scene_Data.light_data offsetof on %d\n", offsetof(struct Scene_Data,light_data));
	// printf("  Scene_Data.instance_data offsetof on %d\n", offsetof(struct Scene_Data,instance_data));

	// printf("  Scene_Data.material_data offsetof on %d\n", offsetof(struct Scene_Data,material_data));
	


	printf("struct InstanceData align on %d\n", _Alignof(struct InstanceData));
	printf("struct InstanceData sizeof on %d\n", sizeof(struct InstanceData));

	printf("struct Camera _Alignof on %d\n", _Alignof(struct CameraData));
	printf("struct Camera sizeof on %d\n", sizeof(struct CameraData));
	printf("struct Camera sizeof % 16 %d\n", sizeof(struct CameraData) % 16);
	if (sizeof(struct CameraData) % 32 != 0) {

		printf("struct is missing %d\n", sizeof(struct CameraData) % 32);
		exit(1);
	}


	// app_info.callbacks[0].callback_resize = Scene_callback_FrameBuffer_Resize;
	// app_info.callbacks[0].dst = &app_info.scene;

	// app_info.callbacks[1].callback_resize = Renderer_callback_FrameBuffer_Resize;
	// app_info.callbacks[1].dst = &app_info.renderer;
	
	
	
	
	
	struct Plaftorm_info platform_info = {
		.ref_inst = &app_info.instance,
		// .callback_resize = app_info.callbacks,
		// .collback_cnt = ARR_LEN(app_info.callbacks),
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

	

	Mess_Init(
		&app_info.device, 
		&app_info.input, 
		&app_info.input_backend,
		&app_info.platform, 
		&app_info.mess
	);


	Asset0_init(&app_info.mess);
	
	Mess_Proc(&app_info.mess);

	Mess_Clean(&app_info.mess);
	// Swapchain_Destroy(&app_info.renderer.swapchain);

	// Renderer_Destroy(&app_info.renderer);

	// Descriptor_Destroy(&app_info.descriptor);

	Device_Destroy(&app_info.device);

	Platform_destroySurface(&app_info.platform,app_info.platform.surface);

	Platform_Shutdown(&app_info.platform);

	Instance_DestroyInstance(&app_info.instance);


	
}

void App_destroy(struct ApplicationContext * app){
	PRINT_FNAME;
}
