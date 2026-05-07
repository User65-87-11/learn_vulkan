#include "util/common.h"
#include "platform.h"
#include "input.h"
#include "scene.h"
#include "renderer.h"
#include "instance.h"
#include "descriptor.h"
#include "device.h"

void mainLoop() {
  PRINT_FNAME;
  
	Instance_CreateInstance();
	
	Platform_InitWindow();

	Device_Create();

	Descriptor_Init();

	Renderer_Init();
	
	int running = 1;
	
	float lastTime  = Platform_GetTime();
     
     while (running) {

   	 	double currentTime = Platform_GetTime();
        float dt = (float)(currentTime - lastTime);
        lastTime = currentTime;
     
         Platform_PollEvents();


         Input_Update();
 
         Scene_Update(dt);
 
         Renderer_Render(currentTime,dt);
 
         if (Platform_ShouldCloseWindow()) {
             running = 0;
         }
     }

     Device_Destroy();

     
     Platform_Shutdown();
     
     Instance_DestroyInstance();
}