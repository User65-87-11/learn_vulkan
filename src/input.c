#include "input.h"
#include "platform.h"

static struct InputState g_input;

static int initialized = 0;

void Input_Update() {

	if (!initialized) {
	    Platform_GetCursorPos(&g_input.cursorPosX, &g_input.cursorPosY);
	    g_input.prevPosX = g_input.cursorPosX;
	    g_input.prevPosY = g_input.cursorPosY;
	    initialized = 1;
	}

    for (int i = 0; i < MAX_MOUSE_BUTTONS; i++) {
        g_input.mouseButtons[i] = Platform_GetMouseButtonState(i);
    }
    for (int i = 0; i < MAX_KEYS; i++) {
        g_input.keys[i] = Platform_GetKeyState(i);
    }
    
    g_input.prevPosX = g_input.cursorPosX;
    g_input.prevPosY = g_input.cursorPosY;
    
    Platform_GetCursorPos(&g_input.cursorPosX, &g_input.cursorPosY);

    g_input.mouseDeltaX = g_input.cursorPosX - g_input.prevPosX;
    g_input.mouseDeltaY = g_input.cursorPosY - g_input.prevPosY;
}


struct InputState * Input_getState(){
	return &g_input;
}