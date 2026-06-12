#pragma once
#include "../mess.h"



void Pipeline_Create_Main(struct Mess * ref);

void Pipeline_Create_HUD(struct Mess * ref);

void Pipeline_Create_Grid(struct Mess * ref);

void Pipeline_destory(struct Device_State * device,struct Pipeline_State * pipeline);


void Pipeline_Create_Axis(struct Mess * ref);
