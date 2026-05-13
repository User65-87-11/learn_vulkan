#include "input.h"
#include "platform.h"
#include <stdio.h>

static struct InputState input;

static int initialized = 0;

void Input_Update() {

	double current_x;
	double current_y;
	Platform_GetCursorPos(&current_x, &current_y);

	input.cursorPosX = current_x;
	input.cursorPosY = current_y;

	if (!initialized) {
		input.prevPosX = current_x;
		input.prevPosY = current_y;
		initialized = 1;
	}

	for (int i = 0; i < MAX_MOUSE_BUTTONS; i++) {
		input.mouseButtons[i] = Platform_GetMouseButtonState(i);
	}
	for (int i = 0; i < MAX_KEYS; i++) {
		input.keys[i] = Platform_GetKeyState(i);
	}

	input.mouseDeltaX = current_x - input.prevPosX;
	input.mouseDeltaY = input.prevPosY - current_y;

	input.mouseDeltaX *= input.sensitivity;
	input.mouseDeltaY *= input.sensitivity;

	input.prevPosX = current_x;
	input.prevPosY = current_y;
}

struct InputState* Input_Get() { return &input; }

void Input_Init() { input.sensitivity = 0.1f; }
