#pragma once
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "descriptor.h"
#include "device2.h"
#include "instance.h"
#include "platform.h"

struct ApplicationContext {
	struct DescriptorContext descriptor;
	struct Device_State device;
	struct Renderer renderer;
	struct Scene scene;
	struct Instance_State instance;
	struct Platform_State platform;

	struct Input_State input;
	struct Input_Backend input_backend;
	
};

void App_start();

void frameResizeCallback(GLFWwindow * window, int width, int height) ;
