#include <GLFW/glfw3.h>
#include <assert.h>
#include <string.h>
#include "scene.h"
#include "input.h"
#include "platform.h"
#include "util/gm_array.h"
#include "util/gm_list.h"
#include "app.h"

// static struct Camera camera;

// static vec3 cameraPos = {1.5f, 0.5f, -1.6f};
// static vec3 cameraFront = {-0.7f, 0.0f, 0.7f};
// static vec3 cameraUp = {0.0f, 1.0f, 0.0f};

// static float yaw = -90.0f;
// static float pitch = 0.0f;
// static float fov = 45.0f;
//

// static struct Scene* scene_ref;

// static struct GmArray array_meshes;

static const float yaw = -90.0f;

static void update_camera(
	struct Scene* scene, struct InputState* input, float dt);

static void update_entities(
	struct Scene* scene, struct InputState* input, float dt);

static void update_keys(
	struct Scene* scene, struct InputState* input, float dt);

void Scene_Init(struct Scene* scene, float aspect_ratio) {
	PRINT_FNAME;
	memset(scene, 0, sizeof(struct Scene));

	// scene_ref = scene;

	scene->camera_data.fov = 45.0f;
	scene->camera_data.near_plane = 0.1f;
	scene->camera_data.far_plane = 100.0f;
	scene->camera_data.aspect_ratio = aspect_ratio;

	scene->camera_data.yaw = yaw;
	scene->camera_data.pitch = 0.0f;

	GLM_VEC3_SET(scene->camera_data.front, -0.7f, 0.0f, 0.7f);
	GLM_VEC3_SET(scene->camera_data.pos, 0.5f, 0.5f, 2.6f);
	GLM_VEC3_SET(scene->camera_data.up, 0.0f, 1.0f, 0.0f);

	// gmArrayInit(&array_meshes, sizeof(struct Mesh), MAX_INSTANCES,
	// sizeof(struct Mesh));

	glm_perspective(glm_rad(scene->camera_data.fov),
		scene->camera_data.aspect_ratio, scene->camera_data.near_plane,
		scene->camera_data.far_plane, scene->camera_data.proj);

	scene->camera_data.proj[1][1] *= -1;
	scene->global_data.frame_cnt = 0x88;

	glm_mat4_identity(scene->camera_data.view);
}

struct Entity* Scene_NewEntity(struct Scene* scene) {
	assert(scene->entities_count < MAX_ENTITIES);
	return &scene->entities[scene->entities_count++];
}

struct Entity* Scene_GetEntity(struct Scene* scene, uint32_t position) {
	assert(scene->entities_count > position);

	return &scene->entities[position];
}

struct Mesh* Scene_NewMesh(struct Scene* scene) {
	assert(scene->mesh_count < MAX_MESHES);
	return &scene->meshes[scene->mesh_count++];
}

struct Mesh* Scene_GetMesh(struct Scene* scene, uint32_t position) {
	assert(scene->mesh_count > position);

	return &scene->meshes[position];
}

struct MaterialData* Scene_NewMaterial(struct Scene* scene) {
	assert(scene->material_count < MAX_MATERIALS);

	return &scene->material_data[scene->material_count++];
}

struct MaterialData* Scene_GetMaterial(struct Scene* scene, uint32_t position) {
	assert(scene->material_count > position);

	return &scene->material_data[position];
}

struct InstanceData* Scene_NewInstanceData(struct Scene* scene) {
	assert(scene->instance_count < MAX_INSTANCES);
	return &scene->instance_data[scene->instance_count++];
}

struct InstanceData* Scene_GeInstanceData(
	struct Scene* scene, uint32_t position) {
	assert(scene->instance_count > position);

	return &scene->instance_data[position];
}

void Scene_Update(struct Scene* scene, float dt) {

	struct InputState* input = Input_Get();
	update_keys(scene, input, dt);
	update_camera(scene, input, dt);
}

static void update_keys(
	struct Scene* scene, struct InputState* input, float dt) {

	if (input->keys[GLFW_KEY_ESCAPE] == GLFW_PRESS) {
		Platform_SetShouldCloseWindow(true);
	}

	float cameraSpeed = 2.5 * dt;
	if (input->keys[GLFW_KEY_W] == GLFW_PRESS) {

		glm_vec3_muladds(
			scene->camera_data.front, cameraSpeed, scene->camera_data.pos);
	}

	if (input->keys[GLFW_KEY_S] == GLFW_PRESS) {
		glm_vec3_mulsubs(
			scene->camera_data.front, cameraSpeed, scene->camera_data.pos);
	}
	if (input->keys[GLFW_KEY_A] == GLFW_PRESS) {
		vec3 temp;
		glm_vec3_crossn(scene->camera_data.front, scene->camera_data.up, temp);
		glm_vec3_mulsubs(temp, cameraSpeed, scene->camera_data.pos);
	}
	if (input->keys[GLFW_KEY_D] == GLFW_PRESS) {
		vec3 temp;
		glm_vec3_crossn(scene->camera_data.front, scene->camera_data.up, temp);
		glm_vec3_muladds(temp, cameraSpeed, scene->camera_data.pos);
	}

	if (input->keys[GLFW_KEY_SPACE] == GLFW_PRESS) {

		glm_vec3_muladds(
			scene->camera_data.up, cameraSpeed, scene->camera_data.pos);
	}
}

static void update_camera(
	struct Scene* scene, struct InputState* input, float dt) {

	scene->camera_data.yaw += input->mouseDeltaX;

	scene->camera_data.pitch += input->mouseDeltaY;

	if (scene->camera_data.pitch > 89.0f) {
		scene->camera_data.pitch = 89.0f;
	}
	if (scene->camera_data.pitch < -89.0f) {
		scene->camera_data.pitch = -89.0f;
	}

	vec3 front;

	front[0] = cos(glm_rad(scene->camera_data.yaw)) *
			   cos(glm_rad(scene->camera_data.pitch));
	front[1] = sin(glm_rad(scene->camera_data.pitch));
	front[2] = sin(glm_rad(scene->camera_data.yaw)) *
			   cos(glm_rad(scene->camera_data.pitch));

	GLM_VEC3_COPY(scene->camera_data.front, front);

	vec3 cameraCenter;
	glm_vec3_add(
		scene->camera_data.pos, scene->camera_data.front, cameraCenter);
	glm_lookat(scene->camera_data.pos, cameraCenter, scene->camera_data.up,
		scene->camera_data.view);

	glm_mat4_mul(scene->camera_data.proj, scene->camera_data.view,
		scene->camera_data.view_proj);
}

static void update_entities(
	struct Scene* scene, struct InputState* input, float dt) {
	for (int i = 0; i < scene->entities_count; i++) {
		struct Entity* entity = &scene->entities[i];
	}
}
