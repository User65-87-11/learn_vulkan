#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cglm/cam.h"
#include "cglm/clipspace/persp_rh_no.h"
#include "cglm/vec3.h"


#include "scene.h"
#include "input.h"
#include "platform.h"
#include "shader_common.h"
#include "util/common.h"


static const float yaw = -230.0f;
static const float pitch = 30.0f;

static void update_camera(
	struct Scene* scene, float dt);



static void update_keys(
	struct Scene* scene,  float dt);

static void camera_perspective_init(
	struct CameraData* cam,
	float fov,
	float near_plane,
	float far_plane,
	float aspect_ratio,
	float yaw,
	float pitch,
	vec3 pos

);

void Scene_Init(struct Scene_Info * info, struct Scene* scene) {
	PRINT_FNAME;
	memset(scene, 0, sizeof(struct Scene));

	
	// scene->data = gm_alloc_aligned(sizeof(*scene->data),32);
	// memset(scene->data, 0, sizeof(*scene->data));
	
	
	scene->ref_device = info->ref_device;
	scene->ref_input = info->ref_input;
	scene->ref_platform = info->ref_platform;

	
	vec3 pos0 = {2.0, -2.0f, -2.0f};


	camera_perspective_init(
		&scene->camera_data, 
		45.0, 0.1f, 100.0f,
		(float)info->width / info->height, 
		yaw, pitch, 
		pos0
	);
	


	
	// scene_ref = scene;

	scene->global_data.frame_cnt = 0x88;
	scene->global_data.framebuffer_size[0] = info->width;	
	scene->global_data.framebuffer_size[1] = info->height;


	// scene->data->global_data.frame_cnt = 0x88;
	// scene->data->global_data.framebuffer_size[0] = info->width;	
	// scene->data->global_data.framebuffer_size[1] = info->height;
	
	
	
}

void Scene_Destroy(struct Scene* scene){
	// if(scene->data!= NULL){
	// 	free(scene->data);
	// 	scene->data= NULL;
	// }
}

static void camera_perspective_init(
	struct CameraData* cam,
	float fov,
	float near_plane,
	float far_plane,
	float aspect_ratio,
	float yaw,
	float pitch,
	vec3 pos
){


	
	cam->fov = fov;
	cam->near_plane = near_plane;
	cam->far_plane = far_plane;
	cam->aspect_ratio = aspect_ratio;

	cam->yaw = yaw;
	cam->pitch = pitch;
	cam->ortho_size = 0.0f;



	

	GLM_VEC3_COPY(cam->pos, pos);
	GLM_VEC3_SET(cam->up,0.0,1.0,0.0);

	printf("CAM: %f,%f,%f\n",cam->pos);


	glm_perspective_rh_no(
		glm_rad(cam->fov),
		cam->aspect_ratio, 
		cam->near_plane,
		cam->far_plane, 
		cam->proj
	);
	
	// glm_mat4_identity(	cam->view);

	vec3 front_;

	front_[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
	front_[1] = sin(glm_rad(cam->pitch));
	front_[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));

	GLM_VEC3_COPY(cam->front, front_);

	vec3 cameraCenter;
	
	glm_vec3_add(
		cam->pos, 
		cam->front, 
		cameraCenter
	);
	
	glm_lookat_rh(
		cam->pos, 
		cameraCenter, 
		cam->up,
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

	
	// struct Input_State* input = scene->ref_input;
	update_keys(scene,  dt);


	
	update_camera(scene,  dt);
}

static void update_keys(
	struct Scene* scene,
	float dt
) {
	struct CameraData* cam = &scene->camera_data;




	if(Input_KeyHeldTime(scene->ref_input, GLFW_KEY_ESCAPE) > 1.0)
	// if(Input_IsKeyPressed(scene->ref_input, GLFW_KEY_ESCAPE))
	// if (scene->ref_input->keys[GLFW_KEY_ESCAPE] == GLFW_PRESS)
	{
		
	
		Platform_SetShouldCloseWindow(scene->ref_platform,true);
	}


	if(Input_IsKeyPressed(scene->ref_input, GLFW_KEY_LEFT_ALT)){

		Platform_ToggleCursor(scene->ref_platform);	
		
	}

	

	float cameraSpeed = 2.5 * dt;
	if(Input_IsKeyDown(scene->ref_input, GLFW_KEY_W)){
//	if (scene->ref_input->keys[GLFW_KEY_W] == GLFW_PRESS) {

		glm_vec3_muladds(
			cam->front, 
			cameraSpeed, 
			cam->pos
		);
	}
if(Input_IsKeyDown(scene->ref_input, GLFW_KEY_S)){
//	if (scene->ref_input->keys[GLFW_KEY_S] == GLFW_PRESS) {
		glm_vec3_mulsubs(
			cam->front, cameraSpeed, cam->pos);
	}
	if(Input_IsKeyDown(scene->ref_input, GLFW_KEY_A)){
//	if (scene->ref_input->keys[GLFW_KEY_A] == GLFW_PRESS) {
		vec3 temp;
		glm_vec3_crossn(cam->front, cam->up, temp);
		glm_vec3_mulsubs(temp, cameraSpeed, cam->pos);
	}
	if(Input_IsKeyDown(scene->ref_input, GLFW_KEY_D)){
//	if (scene->ref_input->keys[GLFW_KEY_D] == GLFW_PRESS) {
		vec3 temp;
		glm_vec3_crossn(cam->front, cam->up, temp);
		glm_vec3_muladds(temp, cameraSpeed, cam->pos);
	}

	if(Input_IsKeyDown(scene->ref_input, GLFW_KEY_SPACE)){
	// if (scene->ref_input->keys[GLFW_KEY_SPACE] == GLFW_PRESS) {

		glm_vec3_mulsubs(
			cam->up, cameraSpeed, cam->pos);
	}
}

static void update_camera(
	struct Scene* scene,  float dt
) {

	
	struct CameraData* cam = &scene->camera_data;


	if(scene->ref_platform->cursor_state != GLFW_CURSOR_NORMAL)
	{
		cam->yaw += scene->ref_input->mouseDeltaX;

		cam->pitch -= scene->ref_input->mouseDeltaY;
	}
	
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
		cam->pos, 
		cam->front, 
		cameraCenter
	);
	
	glm_lookat_rh(
		cam->pos, 
		cameraCenter, 
		cam->up,
		cam->view
	);

	glm_mat4_mul(
		cam->proj, 
		cam->view,
		cam->view_proj
	);
}




static void update_camera_perspective(struct CameraData * cam, float aspect){

	cam->aspect_ratio = aspect;


	
	glm_perspective_rh_no(
		glm_rad(cam->fov),
		cam->aspect_ratio, 
		cam->near_plane,
		cam->far_plane, 
		cam->proj
	);
	// cam->proj[1][1] *= -1.f;

	glm_mat4_mul(
		cam->proj, 
		cam->view,
		cam->view_proj
	);
}


void Scene_callback_FrameBuffer_Resize(void * scene,uint32_t w,uint32_t h){
	PRINT_FNAME;

	PRINT_FNAME;

	struct Scene * s = scene;

	s->global_data.framebuffer_size[0] = w;	
	s->global_data.framebuffer_size[1] = h;

	
	update_camera_perspective(&s->camera_data,(float)w/h);
}
