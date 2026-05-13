#pragma once
#include "input.h"
#include "renderer.h"
#include "scene.h"

struct ApplicationContext {
	struct Renderer renderer;
	struct Scene scene;
};

void App_start();
