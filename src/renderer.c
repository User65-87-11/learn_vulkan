#include <stdint.h>
#include <vulkan/vulkan_core.h>
#include <string.h>

#include "renderer.h"
#include "platform.h"
#include "pipeline.h"
#include "instance.h"
#include "scene.h"
#include "swapchain.h"
#include "vertex.h"
#include "shader.h"
#include "device.h"
#include "util/common.h"
#include "resource.h"
#include "descriptor.h"
#include "shader_common.h"
#include "config.h"
#include "loader/loader.h"

static void createSyncObjects(struct Renderer* renderer);

static void updateBuffers(struct Renderer* renderer,
	struct Scene* scene,
	float time,
	float delta_time);

static void recordCommandBuffer(struct Renderer* renderer,
	struct Scene* scene,
	uint32_t imageIndex,
	uint32_t frameIndex);

static void renderMainPass(struct Renderer* renderer,
	struct Scene* scene,
	uint32_t frame_index,
	uint32_t imageIndex);

void Renderer_Destroy(struct Renderer* renderer) {
	PRINT_FNAME;
	VkDevice device = Device_Get()->logical_device;

	Pipeline_Destroy(device, &renderer->pipeline);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		vkDestroyFence(device, renderer->frames[i].inFlightFence, NULL);

		vkDestroySemaphore(device, renderer->frames[i].imageAvailable, NULL);
		vkDestroySemaphore(device, renderer->frames[i].renderFinished, NULL);

		Resource_FreeBuffer(&renderer->frames[i].buffer_global);
		Resource_FreeBuffer(&renderer->frames[i].buffer_global_camera);
		Resource_FreeBuffer(&renderer->frames[i].buffer_global_light);
		Resource_FreeBuffer(&renderer->frames[i].buffer_instances);
		Resource_FreeImage(&renderer->frames[i].depth_image);
	}
	for (int i = 0; i < renderer->texture_cnt; i++) {
		struct Texture texture = renderer->textures[i];
		Resource_FreeTexture(&texture);
	}
	renderer->texture_cnt = 0;
	Resource_FreeBuffer(&renderer->buffer_index);
	Resource_FreeBuffer(&renderer->buffer_vertex);
	Resource_FreeBuffer(&renderer->buffer_materials);

	// vkFreeDescriptorSets(device, Descriptor_GetContext()->pool, 1,
	// &renderer->desc_set_materials); vkFreeDescriptorSets(device,
	// Descriptor_GetContext()->pool, 1, &renderer->desc_set_samplers);
	// vkFreeDescriptorSets(device, Descriptor_GetContext()->pool,
	// MAX_FRAMES_IN_FLIGHT, renderer->desc_set_globals);
	// vkFreeDescriptorSets(device, Descriptor_GetContext()->pool,
	// MAX_FRAMES_IN_FLIGHT, renderer->desc_set_instances);
}

void Renderer_Init(struct Renderer* renderer) {

	memset(renderer, 0, sizeof(struct Renderer));

	createSyncObjects(renderer);

	VkInstance instance = Instance_Get();
	VkSurfaceKHR surface = Platform_GetSurface();
	struct Device* device = Device_Get();

	VkExtent2D extent;
	Platform_GetFramebufferSize(&extent.width, &extent.height);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		struct FrameData* data = &renderer->frames[i];
		Device_AllocateCommandBuffer(
			device->graphics_pool, &data->commandBuffer);
	}

	Swapchain_Create(&renderer->swapchain, device->logical_device,
		device->physical_device, surface, extent);

	memset(&renderer->pipeline.info, 0, sizeof(renderer->pipeline.info));

	renderer->pipeline.info.vertexShader =
		Shader_CreateFromFile(device->logical_device, DEFAULT_SHADER_VERT);
	renderer->pipeline.info.fragmentShader =
		Shader_CreateFromFile(device->logical_device, DEFAULT_SHADER_FRAG);

	struct DescriptorContext* ctx = Descriptor_GetContext();

	VkDescriptorSetLayout layouts[] = {ctx->globalLayout, ctx->instanceLayout,
		ctx->materialLayout, ctx->samplerLayout};

	renderer->pipeline.info.descriptorSetLayouts = layouts;
	renderer->pipeline.info.descriptorSetLayoutCount = ARR_LEN(layouts);

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		renderer->desc_set_globals[i] = Descriptor_Allocate(ctx->globalLayout);

		renderer->desc_set_instances[i] =
			Descriptor_Allocate(ctx->instanceLayout);
	}
	renderer->desc_set_materials = Descriptor_Allocate(ctx->materialLayout);
	renderer->desc_set_samplers = Descriptor_Allocate(ctx->samplerLayout);

	renderer->pipeline.info.depthFormat = renderer->swapchain.depthFormat;
	renderer->pipeline.info.colorFormat = renderer->swapchain.surfaceFormat;

	Pipeline_CreateGraphics(&renderer->pipeline);

	/*

		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		VK_BUFFER_USAGE_TRANSFER_DST_BIT

		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		VK_MEMORY_PROPERTY_HOST_CACHED_BIT
	 */

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		struct FrameData* f = &renderer->frames[i];

		Resource_CreateBuffer(sizeof(struct GlobalData), BUFFER_UBO_USAGE,
			BUFFER_UBO_PROPS, &f->buffer_global);
		Resource_mapBufferMemory(&f->buffer_global);

		Descriptor_UpdateBuffer(renderer->desc_set_globals[i],
			BINDING_GLOBAL_GLOBAL, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			f->buffer_global.handle, f->buffer_global.size);

		Resource_CreateBuffer(sizeof(struct CameraData)*2, BUFFER_UBO_USAGE,
			BUFFER_UBO_PROPS, &f->buffer_global_camera);
		Resource_mapBufferMemory(&f->buffer_global_camera);

		Descriptor_UpdateBuffer(renderer->desc_set_globals[i],
			BINDING_GLOBAL_CAMERA, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			f->buffer_global_camera.handle, f->buffer_global_camera.size);

		Resource_CreateBuffer(sizeof(struct LightData), BUFFER_UBO_USAGE,
			BUFFER_UBO_PROPS, &f->buffer_global_light);
		Resource_mapBufferMemory(&f->buffer_global_light);

		Descriptor_UpdateBuffer(renderer->desc_set_globals[i],
			BINDING_GLOBAL_LIGHT, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			f->buffer_global_light.handle, f->buffer_global_light.size);

		Resource_CreateBuffer(sizeof(struct InstanceData) * MAX_INSTANCES,
			BUFFER_SSBO_USAGE, BUFFER_SSBO_PROPS, &f->buffer_instances);
		Resource_mapBufferMemory(&f->buffer_instances);

		Descriptor_UpdateBuffer(renderer->desc_set_instances[i], 0,
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, f->buffer_instances.handle,
			f->buffer_instances.size);

		Resource_CreateImage(extent.width, extent.height,
			1, // mip levels
			renderer->swapchain.depthFormat, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &f->depth_image);

		Resource_CreateImageView(&f->depth_image, VK_IMAGE_ASPECT_DEPTH_BIT);
		// Resource_transitionImageLayout(VkCommandBuffer cmdBuffer, VkImage
		// *image, VkImageLayout oldLayout, VkImageLayout newLayout,
		// VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		// VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
		// VkImageAspectFlagBits aspectFlags, uint32_t mipLevels)

		VkCommandBuffer command = Resource_beginSingleTimeCommands();

		Resource_transitionImageLayout(command, &f->depth_image.handle,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
			0, VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
			VK_PIPELINE_STAGE_2_NONE,
			VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT, 1);
		Resource_endSingleTimeCommands(command);
	}

	Resource_CreateBuffer(sizeof(struct MaterialData) * MAX_MATERIALS,
		BUFFER_SSBO_USAGE, BUFFER_SSBO_PROPS, &renderer->buffer_materials);
	Resource_mapBufferMemory(&renderer->buffer_materials);

	Descriptor_UpdateBuffer(renderer->desc_set_materials, 0,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, renderer->buffer_materials.handle,
		renderer->buffer_materials.size);

	Resource_CreateBuffer(MAX_VERTICES * sizeof(struct Vertex),
		BUFFER_VERTEX_USAGE, BUFFER_VERTEX_PROPS, &renderer->buffer_vertex);

	Resource_CreateBuffer(MAX_INDICES * sizeof(uint32_t), BUFFER_INDEX_USAGE,
		BUFFER_INDEX_PROPS, &renderer->buffer_index);
}

void Renderer_AppendToVertexBuffer(
	struct Renderer* renderer, struct Vertex* vertices, uint32_t vertex_cnt) {
	PRINT_FNAME;
	assert(renderer->buffer_vertex_used + vertex_cnt <= MAX_VERTICES);

	printf("vetex_used:%d, vertex_cnt:%d vsize:%d\n",
		renderer->buffer_vertex_used, vertex_cnt, sizeof(struct Vertex));
	Resource_AppendToBuffer(&renderer->buffer_vertex,
		sizeof(struct Vertex) * renderer->buffer_vertex_used, vertices,
		sizeof(struct Vertex) * vertex_cnt);
	renderer->buffer_vertex_used += vertex_cnt;
}

void Renderer_AppendToIndexBuffer(
	struct Renderer* renderer, uint32_t* indices, uint32_t indices_cnt) {
	PRINT_FNAME;
	assert(renderer->buffer_index_used + indices_cnt <= MAX_INDICES);
	printf("index_used:%d, index_cnt:%d\n", renderer->buffer_index_used,
		indices_cnt);
	Resource_AppendToBuffer(&renderer->buffer_index,
		sizeof(uint32_t) * renderer->buffer_index_used, indices,
		sizeof(uint32_t) * indices_cnt);
	renderer->buffer_index_used += indices_cnt;
}

struct Texture* Renderer_NewTexture(
	struct Renderer* renderer, void* data, uint32_t size) {
	PRINT_FNAME;

	struct ImageData imageData;

	struct Texture* texture = &renderer->textures[renderer->texture_cnt++];
	Loader_LoadImageDataFromMemory(data, size, &imageData);
	Resource_CreateTexture(imageData.data, imageData.width, imageData.height,
		VK_FORMAT_R8G8B8A8_SRGB, texture);
	Loader_FreeImageData(&imageData);
	
	return texture;
}

struct Texture* Renderer_GetTexture(
	struct Renderer* renderer, uint32_t position) {
	assert(renderer->texture_cnt > position);

	return &renderer->textures[position];
}

void Renderer_Render(struct Renderer* renderer,
	struct Scene* scene,
	float time,
	float delta_time) {

	uint32_t frame_index = renderer->current_frame;
	struct FrameData* frame = &renderer->frames[frame_index];

	struct Device* device = Device_Get();

	VkResult result = vkWaitForFences(
		device->logical_device, 1, &frame->inFlightFence, VK_TRUE, UINT64_MAX);

	if (result != VK_SUCCESS) {
		EXIT_CLEAN("failed to wait for fence!");
	}

	uint32_t imageIndex = -1;

	result = vkAcquireNextImageKHR(device->logical_device,
		renderer->swapchain.handle, UINT64_MAX, frame->imageAvailable, NULL,
		&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {

		Swapchain_Recreate(&renderer->swapchain, device->logical_device,
			device->physical_device, Platform_GetSurface());

		return;
	}

	// updateGameObjects(frame_index);

	updateBuffers(renderer, scene, time, delta_time);

	vkResetFences(device->logical_device, 1, &frame->inFlightFence);

	recordCommandBuffer(renderer, scene, imageIndex, frame_index);

	VkPipelineStageFlags2 stageMask =
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo2 submitInfo2 = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,

		.waitSemaphoreInfoCount = 1,
		.pWaitSemaphoreInfos =
			&(VkSemaphoreSubmitInfo){
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
				.semaphore = frame->imageAvailable,
				.stageMask = stageMask,
				.value = 0,
				.deviceIndex = 0,

			},

		.commandBufferInfoCount = 1,
		.pCommandBufferInfos =
			&(VkCommandBufferSubmitInfo){
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
				.commandBuffer = frame->commandBuffer,

			},
		.signalSemaphoreInfoCount = 1,
		.pSignalSemaphoreInfos =
			&(VkSemaphoreSubmitInfo){
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
				.semaphore = frame->renderFinished,
				.stageMask = stageMask,

			},
	};

	vkQueueSubmit2(
		device->graphics_queue, 1, &submitInfo2, frame->inFlightFence);

	VkPresentInfoKHR presentInfo = {

		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &frame->renderFinished,
		.swapchainCount = 1,
		.pSwapchains = &renderer->swapchain.handle,
		.pImageIndices = &imageIndex,

	};

	result = vkQueuePresentKHR(device->graphics_queue, &presentInfo);

	if ((result == VK_SUBOPTIMAL_KHR) || (result == VK_ERROR_OUT_OF_DATE_KHR) ||
		renderer->framebuffer_resized) {
		renderer->framebuffer_resized = false;

		Swapchain_Recreate(&renderer->swapchain, device->logical_device,
			device->physical_device, Platform_GetSurface());

		// Swapchain_Recreate(struct Swapchain *sc, VkDevice device,
		// VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
		// recreateSwapChain();
	} else {
		assert(result == VK_SUCCESS);
	}

	renderer->current_frame = (frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
}

static void createSyncObjects(struct Renderer* renderer) {

	PRINT_FNAME;
	struct Device* device = Device_Get();

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		struct FrameData* frame = &renderer->frames[i];

		VkSemaphoreCreateInfo semaphoreCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};

		vkCreateSemaphore(device->logical_device, &semaphoreCreateInfo, NULL,
			&frame->imageAvailable);

		vkCreateSemaphore(device->logical_device, &semaphoreCreateInfo, NULL,
			&frame->renderFinished);

		VkFenceCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT,
		};

		vkCreateFence(
			device->logical_device, &createInfo, NULL, &frame->inFlightFence);
	}
}

static void recordCommandBuffer(struct Renderer* renderer,
	struct Scene* scene,
	uint32_t imageIndex,
	uint32_t frameIndex) {

	struct FrameData* frame = &renderer->frames[frameIndex];

	VkCommandBuffer commandBuffer = frame->commandBuffer;

	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0,
		.pInheritanceInfo = NULL,
	};
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	Resource_transitionImageLayout(commandBuffer,
		&renderer->swapchain.images[imageIndex], VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0,
		VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_IMAGE_ASPECT_COLOR_BIT, 1);

	renderMainPass(renderer, scene, frameIndex, imageIndex);

	Resource_transitionImageLayout(commandBuffer,
		&renderer->swapchain.images[imageIndex],
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		0, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		// VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_NONE, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	vkEndCommandBuffer(commandBuffer);
}

static void renderMainPass(struct Renderer* renderer,
	struct Scene* scene,
	uint32_t frame_index,
	uint32_t imageIndex) {

	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	VkClearValue clearDepth = {{{1.0f, 0}}};

	struct FrameData* frame = &renderer->frames[frame_index];

	VkRenderingAttachmentInfo colorAttachmentsInfos[] = {

		colorAttachmentsInfos[0] =
			(VkRenderingAttachmentInfo){

				.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
				.imageView = renderer->swapchain.image_views[imageIndex],
				.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.clearValue = clearColor

			}

	};

	VkRenderingAttachmentInfo depthAttachmentInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = frame->depth_image.view,
		.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.clearValue = clearDepth};

	VkRenderingInfo renderingInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea = {.offset = {0, 0}, .extent = renderer->swapchain.extent},

		.layerCount = 1,
		.colorAttachmentCount = ARR_LEN(colorAttachmentsInfos),
		.pColorAttachments = colorAttachmentsInfos,

		.pDepthAttachment = &depthAttachmentInfo,
	};

	vkCmdBeginRendering(frame->commandBuffer, &renderingInfo);

	vkCmdBindPipeline(frame->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		renderer->pipeline.handle);

	VkViewport viewPort = {.x = 0,
		.y = 0,
		.width = renderer->swapchain.extent.width,
		.height = renderer->swapchain.extent.height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f};

	vkCmdSetViewport(frame->commandBuffer, 0, 1, &viewPort);

	VkRect2D scissor = {.extent = renderer->swapchain.extent, .offset = {0, 0}};
	vkCmdSetScissor(frame->commandBuffer, 0, 1, &scissor);

	VkDeviceSize offset = 0;

	vkCmdBindVertexBuffers(
		frame->commandBuffer, 0, 1, &renderer->buffer_vertex.handle, &offset);
	vkCmdBindIndexBuffer(frame->commandBuffer, renderer->buffer_index.handle, 0,
		VK_INDEX_TYPE_UINT32);

	VkDescriptorSet dset[4] = {
		renderer->desc_set_globals[frame_index],
		renderer->desc_set_instances[frame_index],
		renderer->desc_set_materials,
		renderer->desc_set_samplers,
	};

	vkCmdBindDescriptorSets(frame->commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipeline.layout, 0,
		ARR_LEN(dset), dset, 0, NULL
	);



	
	for (int i = 0; i < scene->entities_count; i++) {

		struct Entity* entity = &scene->entities[i];

		struct Mesh* mesh = &scene->meshes[entity->mesh_index];

		vkCmdDrawIndexed(
			frame->commandBuffer, 
			mesh->index_count, 1,
			mesh->index_offset, 
			mesh->vertex_offset, 0
		);
	}
	
	// for(int i=0;i<scene->mesh_count;i++)
	// {
	// 	struct Mesh* mesh = &scene->meshes[i];
	// 		printf("  mesh->index_count %d\n",  mesh->index_count)	;

			
	// 	vkCmdDrawIndexed(
	// 	    frame->commandBuffer,
	// 	    mesh->index_count,
	// 	    scene->instance_count,
	// 	    mesh->f,
	// 	    mesh->vertex_offset,
	// 	    0
	// 	);
	// }

	vkCmdEndRendering(frame->commandBuffer);
}

static void updateBuffers(struct Renderer* renderer,
	struct Scene* scene,
	float time,
	float delta_time) {

	struct FrameData* frame = &renderer->frames[renderer->current_frame];

	memcpy(frame->buffer_instances.mapped, scene->instance_data,
		sizeof(scene->instance_data));

	memcpy(frame->buffer_global.mapped, &scene->global_data,
		sizeof(scene->global_data));

	memcpy(frame->buffer_global_camera.mapped, &scene->camera_data,
		sizeof(scene->camera_data));

	memcpy(frame->buffer_global_light.mapped, &scene->light_data,
		sizeof(scene->light_data));

	memcpy(renderer->buffer_materials.mapped, scene->material_data,
		sizeof(scene->material_data));
}
