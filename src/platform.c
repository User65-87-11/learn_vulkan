#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "cglm/cam.h"
#include "cglm/clipspace/ortho_rh_no.h"
#include "cglm/clipspace/persp_rh_no.h"
#include "cglm/clipspace/view_rh_no.h"
#include "vulkan/vk_platform.h"


#include "app.h"
#include "shader_common.h"
#include "common.h"
#include "platform.h"
// #include "renderer.h"
#include "instance.h"

#ifdef _WIN32
#include <direct.h>
#include <stdlib.h>
#else
#include <unistd.h>

#endif


static void frameResizeCallback(GLFWwindow * window, int width, int height);


void Platform_createSurface(
	struct Platform_State* plaftorm, VkSurfaceKHR *surface) {

	VK_CHECK(glfwCreateWindowSurface(
		plaftorm->ref_inst->instance, plaftorm->window, NULL, surface));
}

void Platform_destroySurface(	struct Platform_State * plaftorm, VkSurfaceKHR  surface) {
	
	vkDestroySurfaceKHR(plaftorm->ref_inst->instance, surface, NULL);
}

void Platform_Init(struct Plaftorm_info * info ,struct Platform_State * state) {
	PRINT_FNAME;

	assert(info->collback_cnt <= ARR_LEN(state->callback_resize));
	
	printf("%s %d %d\n", __FUNCTION__, WIDTH, HEIGHT);

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	
	memset(state,0, sizeof(*state));

	printf("callbacks: %d\n",info->collback_cnt);
	
	
	state->window = glfwCreateWindow(info->width, info->height, "Vulkan", NULL, NULL);
	state->ref_inst = info->ref_inst;
	state->cursor_state = info->cursor_state;

	// state->callback_resize = malloc(sizeof(struct Platform_callback)*info->collback_cnt);
	
	for(int i=0;i<info->collback_cnt;i++)
	{
	
		state->callback_resize[i] = info->callback_resize[i];
		state->collback_cnt ++;
	}
	
	


	glfwSetInputMode(state->window, GLFW_CURSOR, state->cursor_state);

	glfwSetWindowUserPointer(state->window, state);

	glfwSetFramebufferSizeCallback(state->window,frameResizeCallback);
}

void Platform_ToggleCursor(struct Platform_State * plaftorm){
	if(plaftorm->cursor_state == GLFW_CURSOR_DISABLED){
		plaftorm->cursor_state = GLFW_CURSOR_NORMAL;
	}else{
		plaftorm->cursor_state = GLFW_CURSOR_DISABLED;
	}

	glfwSetInputMode(plaftorm->window, GLFW_CURSOR, plaftorm->cursor_state);
}

void Platform_WaitForEvents(struct Platform_State * platform) { glfwWaitEvents(); }

void Platform_GetFramebufferSize(struct Platform_State * platform, uint32_t* width, uint32_t* height) {

	glfwGetFramebufferSize(platform->window, (int*)width, (int*)height);
}



void Platform_Shutdown(struct Platform_State * plaftorm) { 
	glfwDestroyWindow(plaftorm->window);
	
}


void Platform_PollEvents(struct Platform_State* plaftorm) { glfwPollEvents(); }

int Platform_ShouldCloseWindow(struct Platform_State* plaftorm) {
	return glfwWindowShouldClose(plaftorm->window);
}

double Platform_GetTime(struct Platform_State* plaftorm) {
	return glfwGetTime();
}


void Platform_SetShouldCloseWindow(struct Platform_State * plaftorm,uint32_t value) {

	glfwSetWindowShouldClose(plaftorm->window, value);
}



void Platform_GetMousePosition(void* window, double* x, double* y){

	glfwGetCursorPos((GLFWwindow*)window, x, y);
}
int Platform_GetMouseButton(void* window, int button){
	
	return glfwGetMouseButton((GLFWwindow*)window, button);
}
int Platform_GetKey(void* window, int key){
	
	 return glfwGetKey((GLFWwindow*)window, key);
}
void Platform_setFrameResizeCallBack(
	struct Platform_State * platform,
	struct Platform_callback * callbacks,
	uint32_t callback_num
){
	assert(callback_num <= 4);
	for(int i=0;i<callback_num;i++)
	{
		platform->callback_resize[i] = callbacks[i];
		platform->collback_cnt ++;
	}
}
static void frameResizeCallback(GLFWwindow * window, int width, int height){
	struct Platform_State * platform = glfwGetWindowUserPointer(window);
//void (*callback)(void * window, uint32_t w,uint32_t h) 
	for(int i=0;i<platform->collback_cnt;i++)
	{
		struct Platform_callback * callback = 	&platform->callback_resize[i];
		callback->callback_resize(callback->dst,width,height);
		// platform->callback_resize[i](window,width,height);
			
	}

}
