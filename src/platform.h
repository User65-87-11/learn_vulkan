#pragma once
#include <stdint.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>


#include "instance.h"

#ifdef _WIN32
#include <direct.h>
#include <stdlib.h>
#else
#include <unistd.h>
#endif

// #include "app.h"

#define WIDTH 800

#define HEIGHT 600

struct Platform_callback{

	void (*callback_resize )(void *dst, uint32_t width, uint32_t height);
	void *dst;
	
	
};

struct Plaftorm_info{
	struct Instance_State * ref_inst;
	
	// void (**callback_resize )(void *arg0,void *arg1, uint32_t width, uint32_t height)  ;
	struct Platform_callback* callback_resize;
	uint32_t collback_cnt;

	uint32_t width;
	uint32_t height;

	int cursor_state;
};
struct Platform_State{
	struct Instance_State * ref_inst;


	
	struct Platform_callback* callback_resize;
	uint32_t collback_cnt;

	
	void *  window;
	VkSurfaceKHR surface;

	int cursor_state ;

};

void Platform_Init(struct Plaftorm_info *info, struct Platform_State* context);

void Platform_WaitForEvents(struct Platform_State *info);


void Platform_Shutdown(struct Platform_State *info);
// void Platform_procMouseInput(GLFWwindow *window);

// void Platform_mouseCallback(GLFWwindow *window, double xposIn, double
// yposIn);

void Platform_createSurface(struct Platform_State * plaftorm,VkSurfaceKHR *surface);

void Platform_destroySurface(struct Platform_State * plaftorm,VkSurfaceKHR sufrace);

// VkSurfaceKHR Platform_GetSurface();

// int Platform_GetMouseButtonState(struct Platform_State * plaftorm,int button);

// void Platform_GetCursorPos(struct Platform_State * plaftorm,double* x, double* y);

float Platform_GetAspectRatio(struct Platform_State * plaftorm);

// int Platform_GetKeyState(struct Platform_State * plaftorm,int key);

void Platform_PollEvents(struct Platform_State * plaftorm);

float Platform_GetTime(struct Platform_State * plaftorm);

int Platform_ShouldCloseWindow(struct Platform_State * plaftorm);

void Platform_SetShouldCloseWindow(struct Platform_State * plaftorm,uint32_t value);



// bool Platform_HasFrameBufferResized(struct Platform_State * plaftorm);

void Platform_GetFramebufferSize(struct Platform_State * plaftorm,uint32_t* width, uint32_t* height);

void Platform_GetMousePosition(void* window, double* x, double* y);

void Platform_ToggleCursor(struct Platform_State * plaftorm);

int Platform_GetMouseButton(void* window, int button);
int Platform_GetKey(void* window, int key);
