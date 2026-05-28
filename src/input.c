#include <stdio.h>

#include "input.h"
#include "platform.h"


void Input_Update(struct Input_State * input, struct Input_Backend * backend) {

	double current_x;
	double current_y;

	backend->get_mouse_pos(backend->window,&current_x, &current_y);
	

	
	if (!input->initialized) {
		input->prevPosX = current_x;
		input->prevPosY = current_y;
		input->initialized = 1;
	}

	input->prevPosX = 	input->cursorPosX;
	input->prevPosY = 	input->cursorPosY;
	
	input->cursorPosX = current_x;
	input->cursorPosY = current_y;


	for (int i = 0; i < MAX_MOUSE_BUTTONS; i++) {
		input->mouseButtons_prev[i] = input->mouseButtons[i];
		input->mouseButtons[i] = backend->get_mouse_button(backend->window,i);
		
	}
	for (int i = 0; i < MAX_KEYS; i++) {
		
		input->keys_prev[i]  = 	input->keys[i] ;
		input->keys[i] = backend->get_key(backend->window,i);
		
	}



	

	
	input->mouseDeltaX = current_x - input->prevPosX;
	input->mouseDeltaY = input->prevPosY - current_y;

	input->mouseDeltaX *= input->sensitivity;
	input->mouseDeltaY *= input->sensitivity;


}
bool Input_IsMouseDown(struct Input_State * input,int button){
	if(input->mouseButtons[button] == GLFW_PRESS ){
		return true;
	}
	return false;
}
bool Input_IsMouseUp(struct Input_State * input,int button){
	if(input->mouseButtons[button] == GLFW_PRESS ){
		return true;
	}
	return false;
}
bool Input_IsMousePressed(struct Input_State * input,int button){

	if(input->mouseButtons_prev[button] == GLFW_PRESS 
		&& input->mouseButtons[button] == GLFW_RELEASE
	){
		return true;
	}
	return false;
}

void Input_Init(struct Input_State * input) 
{ 
	input->sensitivity = 0.1f; 
	input->initialized = 0;
}

bool Input_IsKeyPressed(struct Input_State * input,int key){

	if(input->keys_prev[key] == GLFW_PRESS && input->keys[key] == GLFW_RELEASE){
		return true;
	}

	return false;
}


bool Input_IsKeyDown(struct Input_State * input,int key){
	if(input->keys[key] == GLFW_PRESS ){
		return true;
	}

	return false;
}
bool Input_IsKeyUp(struct Input_State * input,int key){
	if( input->keys[key] == GLFW_RELEASE){
		return true;
	}

	return false;
}
