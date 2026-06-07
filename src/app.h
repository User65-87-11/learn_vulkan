#pragma once
#include "input.h"
// #include "renderer.h"
// #include "scene.h"
#include "descriptor.h"
#include "device.h"
#include "instance.h"
#include "platform.h"
#include "mess.h"

struct ApplicationContext {
	struct Device_State device;
	
	// struct DescriptorContext descriptor;
	// struct Renderer renderer;
	// struct Scene scene;
	
	struct Instance_State instance;
	struct Platform_State platform;

	struct Input_State input;
	struct Input_Backend input_backend;




	struct Mess mess;
	
};

void App_start();

void App_destroy(struct ApplicationContext * app);
// void frameResizeCallback(GLFWwindow * window, int width, int height) ;
