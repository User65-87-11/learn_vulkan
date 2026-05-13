#pragma once

#define MAX_MOUSE_BUTTONS 10

#define MAX_KEYS 400

struct InputState {
	float mouseDeltaX;
	float mouseDeltaY;
	double cursorPosX;
	double cursorPosY;
	double prevPosX;
	double prevPosY;
	float sensitivity;
	int mouseButtons[MAX_MOUSE_BUTTONS];
	int keys[MAX_KEYS];
};

struct InputState* Input_Get();
void Input_Update();
void Input_Init();
