#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include "util/common.h"
#include "platform.h"
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

static bool framebufferResized = false;

static float posX, posY;

static VkSurfaceKHR surface = VK_NULL_HANDLE;
static VkInstance instance = VK_NULL_HANDLE;


static VkSurfaceCapabilitiesKHR surfaceCapabilities;


static void  mouseCallback(GLFWwindow *window, double xposIn, double yposIn) ;

static void initWindow();

static void framebufferResizeCallback(GLFWwindow *win, int w, int h) {
  framebufferResized = true;
}

void Platform_InitWindow(){
	instance = Instance_getInstance();
	initWindow();
	glfwCreateWindowSurface(instance, window, NULL, &surface);
	
}
void Platform_WaitForEvents(){
	 glfwWaitEvents();
}
void Platform_GetFramebufferSize(uint32_t* width, uint32_t* height){

	 glfwGetFramebufferSize(window, width, height);
}

void Platform_Shutdown(){

	vkDestroySurfaceKHR(instance, surface, NULL);
	glfwDestroyWindow(window);
}

VkSurfaceKHR Platform_GetSurface(){
	
	return surface;
}
static void initWindow() {

  printf("%s %d %d\n", __FUNCTION__, WIDTH, HEIGHT);

  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);

 

  posX = WIDTH >> 1;
  posY = HEIGHT >> 1;

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetCursorPosCallback(window, mouseCallback);

  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
  
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
	*x = posX;
	*y = posY;
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

