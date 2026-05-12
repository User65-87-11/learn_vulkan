#include "src/app.h"
#include "vulkan/vk_platform.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include "util/common.h"
#include "platform.h"
#include "renderer.h"
#include "instance.h"

#ifdef _WIN32
#include <direct.h>
#include <stdlib.h>
#else
#include <unistd.h>

#endif

static GLFWwindow *window = NULL;

// static bool leftPressed = false;
// static bool firstMouse = true;
// static float prevX, prevY;
// static float dx,dy;
// static float lastX = 800.0f / 2.0;
// static float lastY = 600.0 / 2.0;

// static double mouseX, mouseY;

static float posX, posY;

static VkSurfaceKHR surface = VK_NULL_HANDLE;

// static VkInstance instance = VK_NULL_HANDLE;


static VkSurfaceCapabilitiesKHR surfaceCapabilities;


static void  mouseCallback(GLFWwindow *window, double xposIn, double yposIn) ;

static void frameResizeCallback(
	GLFWwindow* window,
    int width,
    int height
);



void Platform_createSurface(VkInstance instance){
	VkResult result = glfwCreateWindowSurface(instance, window, NULL, &surface);
	
}
void Platform_destroySurface(VkInstance instance)
{
		vkDestroySurfaceKHR(instance, surface, NULL);
}
void Platform_InitWindow(struct ApplicationContext * app){
	PRINT_FNAME;
	
	// instance = Instance_getInstance();
 printf("%s %d %d\n", __FUNCTION__, WIDTH, HEIGHT);

  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);

 

  posX = WIDTH >> 1;
  posY = HEIGHT >> 1;

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glfwSetCursorPosCallback(window, mouseCallback);

  glfwSetWindowUserPointer(window, app);
  
  glfwSetFramebufferSizeCallback(window, frameResizeCallback);
  
  

}
static void frameResizeCallback(
	GLFWwindow* window,
    int width,
    int height
){
 	struct ApplicationContext * app = glfwGetWindowUserPointer(window);

    app->renderer.framebuffer_resized = true;
    app->scene.camera_data.aspect_ratio = Platform_GetAspectRatio();
   	glm_perspective(
    	glm_rad( app->scene.camera_data.fov),
		app->scene.camera_data.aspect_ratio, 
		app->scene.camera_data.near_plane,
		app->scene.camera_data.far_plane, 
		app->scene.camera_data.proj
	);
	app->scene.camera_data.proj[1][1] *= -1.f;
}
void Platform_WaitForEvents(){
	 glfwWaitEvents();
}
void Platform_GetFramebufferSize(uint32_t* width, uint32_t* height){

	 glfwGetFramebufferSize(window, width, height);
}

float Platform_GetAspectRatio(){
	VkExtent2D extent;
	Platform_GetFramebufferSize(&extent.width,&extent.height);
  
	float aspect_ratio = (float)extent.width / (float)extent.height;

	return aspect_ratio;
}

void Platform_Shutdown(){


	glfwDestroyWindow(window);
}

VkSurfaceKHR Platform_GetSurface(){
	
	return surface;
}
static void initWindow() {


}



void Platform_PollEvents(){

	glfwPollEvents();
}
int Platform_ShouldCloseWindow(){
	return glfwWindowShouldClose(window) ;
}
float Platform_GetTime(){
	return  glfwGetTime();
}



// void createSurface(VkInstance instance,VkSurfaceKHR * surface) {
//   PRINT_FNAME;

//   glfwCreateWindowSurface(instance, window, NULL, surface);
// }

void Platform_GetCursorPos(double* x, double* y){
	glfwGetCursorPos(window, x, y);
	// *x = posX;
	// *y = posY;
}

static void mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {

	posX = xposIn;
	posY = yposIn;
  // float xpos = xposIn;
  // float ypos = yposIn;

  // if (firstMouse) {
  //   lastX = xpos;
  //   lastY = ypos;
  //   firstMouse = false;
  // }

  // float xoffset = xpos - lastX;
  // float yoffset = lastY - ypos;
  // lastX = xpos;
  // lastY = ypos;

  // float sensitivity = 0.1f;
  // xoffset *= sensitivity;
  // yoffset *= sensitivity;

}

int Platform_GetMouseButtonState(int button){
	
	 return glfwGetMouseButton(window, button);
}
int Platform_GetKeyState(int key){

	return glfwGetKey(window, key);
}
void Platform_SetShouldCloseWindow( uint32_t value){

	 glfwSetWindowShouldClose(window, value);
}

