#include <GLFW/glfw3.h>
#include "scene.h"
#include "input.h"
#include "src/platform.h"

static struct Camera camera;


// static vec3 cameraPos = {1.5f, 0.5f, -1.6f};
// static vec3 cameraFront = {-0.7f, 0.0f, 0.7f};
// static vec3 cameraUp = {0.0f, 1.0f, 0.0f};


// static float yaw = -90.0f;
// static float pitch = 0.0f;
// static float fov = 45.0f;
void Scene_Init(){

	camera.fov =  45.0f;
	camera.fov =  0.0f;
	camera.fov =  -90.0f;

	GLM_VEC3_SET(camera.front, -0.7f, 0.0f, 0.7f);
	GLM_VEC3_SET(camera.pos, 1.5f, 0.5f, -1.6f);
	GLM_VEC3_SET(camera.up, 0.0f, 1.0f, 0.0f);
}

struct Camera * Scene_GetCamera(){
	return &camera;
}

static void update_keys(struct InputState * input, float dt){


  if (input->keys[GLFW_KEY_ESCAPE] == GLFW_PRESS)
  {
 		Platform_SetShouldCloseWindow(true);
  }
    // glfwSetWindowShouldClose(window, true);

  bool leftPressed = false;

  if(input->mouseButtons[GLFW_MOUSE_BUTTON_LEFT] ==  GLFW_PRESS)
  {
    leftPressed = true;
  
  }
  // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
  // }

  float cameraSpeed = 2.5 * dt;
  if (input->keys[GLFW_KEY_W] == GLFW_PRESS) {

  
    glm_vec3_muladds(camera.front, cameraSpeed, camera.pos);
  }

  if (input->keys[ GLFW_KEY_S] == GLFW_PRESS) {
    glm_vec3_mulsubs(camera.front, cameraSpeed, camera.pos);
  }
  if (input->keys[GLFW_KEY_A] == GLFW_PRESS) {
    vec3 temp;
    glm_vec3_crossn(camera.front, camera.up, temp);
    glm_vec3_mulsubs(temp, cameraSpeed, camera.pos);
  }
  if (input->keys[GLFW_KEY_D] == GLFW_PRESS) {
    vec3 temp;
    glm_vec3_crossn(camera.front, camera.up, temp);
    glm_vec3_muladds(temp, cameraSpeed, camera.pos);
  }

  if (input->keys[GLFW_KEY_SPACE] == GLFW_PRESS) {

    glm_vec3_muladds(camera.up, cameraSpeed, camera.pos);
  }

}
static void update_camera(struct Camera* cam, struct InputState * input, float dt){
	
	camera.yaw += input->mouseDeltaX;

	
	camera. pitch += input->mouseDeltaY;
	
	
	if(camera.pitch > 89.0f)
	{
		camera.pitch = 89.0f;
	}
	if(camera.pitch < -89.0f)
	{
		camera.pitch = -89.0f;
	}

  vec3 front;

 
  front[0] = cos(glm_rad( camera.yaw)) * cos(glm_rad( camera.pitch));
  front[1] = sin(glm_rad( camera.pitch));
  front[2] = sin(glm_rad( camera.yaw)) * cos(glm_rad( camera.pitch));

  GLM_VEC3_COPY(camera.front, front);
}

void Scene_Update(float dt){
	struct InputState * input = Input_getState();
	
	update_camera(&camera,input,dt);
	update_keys(input,dt);
	
}