#pragma once


#define MAX_MOUSE_BUTTONS 10

#define MAX_KEYS 400

struct Input_State {
	float mouseDeltaX;
	float mouseDeltaY;
	double cursorPosX;
	double cursorPosY;
	double prevPosX;
	double prevPosY;
	float sensitivity;

	
	
	short mouseButtons_prev[MAX_MOUSE_BUTTONS];
	short mouseButtons[MAX_MOUSE_BUTTONS];
	short keys_prev[MAX_KEYS];
	short keys[MAX_KEYS];

	double keyHeldTime[MAX_KEYS];
    double mouseHeldTime[MAX_MOUSE_BUTTONS];
	
	int initialized ;


	
};
struct Input_Backend{
    void (*get_mouse_pos)(void* window, double* x, double* y);
    int  (*get_mouse_button)(void* window, int button);
    int  (*get_key)(void* window, int key);
    
    void* window;
} ;


void Input_Update(struct Input_State * input, struct Input_Backend * backend, double delta);
void Input_Init(struct Input_State * input);

bool Input_IsKeyPressed(struct Input_State * input,int key);
bool Input_IsKeyDown(struct Input_State * input,int key);
bool Input_IsKeyUp(struct Input_State * input,int key);

bool Input_IsMousePressed(struct Input_State * input,int button);
bool Input_IsMouseDown(struct Input_State * input,int button);
bool Input_IsMouseUp(struct Input_State * input,int button);


void Input_MouseHeldTime_Reset(struct Input_State * input,int button);
void Input_KeyHeldTime_Reset(struct Input_State * input,int key);
double Input_KeyHeldTime(struct Input_State * input,int key);
double Input_MouseHeldTime(struct Input_State * input,int button);
