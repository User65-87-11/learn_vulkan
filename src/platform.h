#pragma once
#include <stdint.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>


#ifdef _WIN32
#include <direct.h>
#include <stdlib.h>
#else
#include <unistd.h>

#endif


const uint32_t WIDTH = 800;

const uint32_t HEIGHT = 600;



void Platform_InitWindow();

void Platform_GetFramebufferSize(uint32_t* width, uint32_t* height);

void Platform_WaitForEvents();
void Platform_Shutdown();
// void Platform_procMouseInput(GLFWwindow *window);

// void Platform_mouseCallback(GLFWwindow *window, double xposIn, double yposIn);


VkSurfaceKHR Platform_GetSurface();

int Platform_GetMouseButtonState(int button);

void Platform_GetCursorPos(double* x, double* y);

int Platform_GetKeyState(int key);

void Platform_PollEvents();

float Platform_GetTime();

int Platform_ShouldCloseWindow();

void Platform_SetShouldCloseWindow( uint32_t value);