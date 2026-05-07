#pragma once
#include <vulkan/vulkan_core.h>
#include "resource.h"


struct FrameData {
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    VkSemaphore imageAvailable;
    VkSemaphore renderFinished;

    VkFence inFlightFence;

    uint32_t imageIndex;

    
    struct Buffer ubo_global;
    struct Buffer ssbo_instances;
    struct Image depth_image;
   
};

void Renderer_Init();

void Renderer_Render(float time, float delta_time);