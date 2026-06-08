#include "pipeline.h"
#include "../device.h"


void Pipeline_destory(struct Device_State * device,struct Pipeline_State * pipeline){
	
		vkDestroyShaderModule(device->logical_device, pipeline->fragmentShader, NULL);
		vkDestroyShaderModule(device->logical_device, pipeline->vertexShader, NULL);
		vkDestroyPipelineLayout(device->logical_device, pipeline->layout, NULL);
		vkDestroyPipeline(device->logical_device, pipeline->handle, NULL);
}
