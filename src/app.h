#pragma once
#include "renderer.h"
#include "scene.h"
#include "input.h"

struct ApplicationContext {
    struct Renderer renderer;
    struct Scene scene;
   
};



void App_start();