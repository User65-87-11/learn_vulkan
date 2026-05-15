#include <GLFW/glfw3.h>
#include <assert.h>
#include <string.h>
#include "scene.h"
#include "cglm/cam.h"
#include "cglm/clipspace/persp_rh_no.h"
#include "input.h"
#include "platform.h"
#include "src/shader_common.h"
#include "src/util/common.h"


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



static void update_keys(
	struct Scene* scene, struct InputState* input, float dt);

static void camera_perspective_init(
	struct CameraData* cam,
	float fov,
	float near_plane,
	float far_plane,
	float aspect_ratio,
	float yaw,
	float pitch,
	vec3 front,
	vec3 pos,
	vec3 up
);
static void camera_ortho_init(
	struct CameraData* cam,
	float aspect_ratio,
	float ortho_size
);

void Scene_Init(struct Scene* scene, uint32_t width,uint32_t height) {
	PRINT_FNAME;
	memset(scene, 0, sizeof(struct Scene));

	// cam->fov = 45.0f;
	// cam->near_plane = 0.1f;
	// cam->far_plane = 100.0f;
	// cam->aspect_ratio = (float)width/height;

	// cam->yaw = yaw;
	// cam->pitch = 0.0f;

	// GLM_VEC3_SET(cam->front, -0.7f, 0.0f, 0.7f);
	// GLM_VEC3_SET(cam->pos, 0.5f, 0.5f, 2.6f);
	// GLM_VEC3_SET(cam->up, 0.0f, 1.0f, 0.0f);

	vec3 pos0 = {-0.7f, 0.0f, 0.7f};
	vec3 front0 = { 0.5f, 0.5f, 2.6f};
	vec3 up0 = { 0.0f, 1.0f, 0.0f};
	camera_perspective_init(
		&scene->camera_data[0], 45.0, 0.1f, 100.0f,
		(float)width / height, yaw, 0.0f, pos0, front0, up0
	);


	camera_ortho_init(&scene->camera_data[1],(float)width / height,10.0);

	
	// scene_ref = scene;

	scene->global_data.frame_cnt = 0x88;
	scene->global_data.framebuffer_size[0] = width;	
	scene->global_data.framebuffer_size[1] = height;

	
}

static void camera_perspective_init(
	struct CameraData* cam,
	float fov,
	float near_plane,
	float far_plane,
	float aspect_ratio,
	float yaw,
	float pitch,
	vec3 front,
	vec3 pos,
	vec3 up
){

	
	// cam->fov = 45.0f;
	// cam->near_plane = 0.1f;
	// cam->far_plane = 100.0f;
	// cam->aspect_ratio = (float)width/height;

	// cam->yaw = yaw;
	// cam->pitch = 0.0f;

	// GLM_VEC3_SET(cam->front, -0.7f, 0.0f, 0.7f);
	// GLM_VEC3_SET(cam->pos, 0.5f, 0.5f, 2.6f);
	// GLM_VEC3_SET(cam->up, 0.0f, 1.0f, 0.0f);

	// // gmArrayInit(&array_meshes, sizeof(struct Mesh), MAX_INSTANCES,
	// // sizeof(struct Mesh));

	// glm_perspective(
	// 	glm_rad(cam->fov),
	// 	cam->aspect_ratio, 
	// 	cam->near_plane,
	// 	cam->far_plane, 
	// 	cam->proj
	// );

	// cam->proj[1][1] *= -1;

	
	cam->fov = fov;
	cam->near_plane = near_plane;
	cam->far_plane = far_plane;
	cam->aspect_ratio = aspect_ratio;

	cam->yaw = yaw;
	cam->pitch = 0.0f;
	cam->ortho_size = 0.0f;

	GLM_VEC3_COPY(cam->front, front);
	GLM_VEC3_COPY(cam->pos, pos);
	GLM_VEC3_COPY(cam->up, up);

	

	glm_perspective_rh_no(
		glm_rad(cam->fov),
		cam->aspect_ratio, 
		cam->near_plane,
		cam->far_plane, 
		cam->proj
	);
	// glm_perspective_rh_no(float fovy, float aspect, float nearZ, float farZ, vec4 *dest)
	glm_mat4_identity(	cam->view);
	// cam->proj[1][1] *= -1;
}
static void camera_ortho_init(
	struct CameraData* cam,
	float aspect_ratio,
	float ortho_size
){

	cam->ortho_size = ortho_size;
	cam->aspect_ratio = aspect_ratio;
	
	float top    = ortho_size;
	float bottom = -ortho_size;
	
	float right = ortho_size * aspect_ratio;
	float left  = -right;


	glm_ortho_rh_no(left,right, bottom, top, -10.0, 10.0, cam->proj);

	glm_lookat_rh(
  		(vec3){1, 1, -1},
	    (vec3){0, 0, 0},
	    (vec3){0, 1, 0},
	    cam->view
	);
	glm_mat4_mul(
		cam->proj, 
		cam->view,
		cam->view_proj
	);
}



struct Mesh* Scene_NewMesh(struct Scene* scene) {
	assert(scene->mesh_count < MAX_MESHES);
	scene->meshes[scene->mesh_count].instance_cnt =  UNSET_VALUE;
	scene->meshes[scene->mesh_count].instance_offset =  UNSET_VALUE;
	return &scene->meshes[scene->mesh_count++];
}

struct Mesh* Scene_GetMesh(struct Scene* scene, uint32_t position) {
	assert(scene->mesh_count > position);

	return &scene->meshes[position];
}

struct MaterialData* Scene_NewMaterial(struct Scene* scene) {
	assert(scene->material_count < MAX_MATERIALS);
	scene->material_data[scene->material_count].base_color_texture_idx = UNSET_VALUE;
	scene->material_data[scene->material_count].metallic_roughness_texture_idx = UNSET_VALUE;	
	return &scene->material_data[scene->material_count++];
}

struct MaterialData* Scene_GetMaterial(struct Scene* scene, uint32_t position) {
	assert(scene->material_count > position);

	return &scene->material_data[position];
}

struct InstanceData* Scene_NewInstanceData(struct Scene* scene) {
	assert(scene->instance_count < MAX_INSTANCES);
	scene->instance_data[scene->instance_count].material_idx = UNSET_VALUE;
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
	struct Scene* scene, 
	struct InputState* input, 
	float dt
) {
	struct CameraData* cam = &scene->camera_data[0];
	
	if (input->keys[GLFW_KEY_ESCAPE] == GLFW_PRESS) {
		Platform_SetShouldCloseWindow(true);
	}

	float cameraSpeed = 2.5 * dt;
	if (input->keys[GLFW_KEY_W] == GLFW_PRESS) {

		glm_vec3_muladds(
			cam->front, cameraSpeed, cam->pos);
	}

	if (input->keys[GLFW_KEY_S] == GLFW_PRESS) {
		glm_vec3_mulsubs(
			cam->front, cameraSpeed, cam->pos);
	}
	if (input->keys[GLFW_KEY_A] == GLFW_PRESS) {
		vec3 temp;
		glm_vec3_crossn(cam->front, cam->up, temp);
		glm_vec3_mulsubs(temp, cameraSpeed, cam->pos);
	}
	if (input->keys[GLFW_KEY_D] == GLFW_PRESS) {
		vec3 temp;
		glm_vec3_crossn(cam->front, cam->up, temp);
		glm_vec3_muladds(temp, cameraSpeed, cam->pos);
	}

	if (input->keys[GLFW_KEY_SPACE] == GLFW_PRESS) {

		glm_vec3_mulsubs(
			cam->up, cameraSpeed, cam->pos);
	}
}

static void update_camera(
	struct Scene* scene, struct InputState* input, float dt
) {
	struct CameraData* cam = &scene->camera_data[0];
	
	cam->yaw += input->mouseDeltaX;

	cam->pitch -= input->mouseDeltaY;

	if (cam->pitch > 89.0f) {
		cam->pitch = 89.0f;
	}
	if (cam->pitch < -89.0f) {
		cam->pitch = -89.0f;
	}

	vec3 front;

	front[0] = cos(glm_rad(cam->yaw)) *
			   cos(glm_rad(cam->pitch));
	front[1] = sin(glm_rad(cam->pitch));
	front[2] = sin(glm_rad(cam->yaw)) *
			   cos(glm_rad(cam->pitch));

	GLM_VEC3_COPY(cam->front, front);

	vec3 cameraCenter;
	glm_vec3_add(
		cam->pos, cam->front, cameraCenter);
	glm_lookat_rh(cam->pos, cameraCenter, cam->up,
		cam->view);

	glm_mat4_mul(cam->proj, cam->view,
		cam->view_proj);
}
