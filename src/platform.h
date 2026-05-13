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

#include "app.h"

#define WIDTH 800

#define HEIGHT 600

void Platform_InitWindow(struct ApplicationContext* context);

void Platform_WaitForEvents();
void Platform_Shutdown();
// void Platform_procMouseInput(GLFWwindow *window);

// void Platform_mouseCallback(GLFWwindow *window, double xposIn, double
// yposIn);

void Platform_createSurface(VkInstance instance);
void Platform_destroySurface(VkInstance instance);

VkSurfaceKHR Platform_GetSurface();

int Platform_GetMouseButtonState(int button);

void Platform_GetCursorPos(double* x, double* y);

float Platform_GetAspectRatio();

int Platform_GetKeyState(int key);

void Platform_PollEvents();

float Platform_GetTime();

int Platform_ShouldCloseWindow();

void Platform_SetShouldCloseWindow(uint32_t value);

bool Platform_HasFrameBufferResized();

void Platform_GetFramebufferSize(uint32_t* width, uint32_t* height);
