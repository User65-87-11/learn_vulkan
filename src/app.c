#include "app.h"
#include "assets/asset0.h"
#include "descriptor.h"
#include "device.h"
#include "input.h"
#include "instance.h"
#include "platform.h"
#include "renderer.h"
#include "scene.h"
#include "src/shader_common.h"
#include "util/common.h"

struct ApplicationContext app_context;

static void mainLoop();

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

	Platform_InitWindow(&app_context);

	Instance_Create();

	Platform_createSurface(Instance_Get());

	Input_Init();

	Device_Create();

	Descriptor_Init();

	Renderer_Init(&app_context.renderer);

	uint32_t width,height;
 	Platform_GetFramebufferSize(&width,&height);
	Scene_Init(&app_context.scene,width,height);

	Asset0_init(&app_context);

	int running = 1;

	float lastTime = Platform_GetTime();

	while (running) {

		double currentTime = Platform_GetTime();
		float dt = (float)(currentTime - lastTime);
		lastTime = currentTime;

		Platform_PollEvents();

		Input_Update();

		Scene_Update(&app_context.scene, dt);

		Renderer_Render(
			&app_context.renderer, &app_context.scene, currentTime, dt);

		if (Platform_ShouldCloseWindow()) {
			running = 0;
		}
	}

	Swapchain_Destroy(&app_context.renderer.swapchain);

	Renderer_Destroy(&app_context.renderer);

	Descriptor_Destroy();

	Device_Destroy();

	Platform_destroySurface(Instance_Get());

	Platform_Shutdown();

	Instance_DestroyInstance();
}
