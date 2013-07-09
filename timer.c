#include "timer.h"

float time_delta;

int Time_Init()
{
    time_delta = 0.0f;
    return 0;
}

void Time_Update()
{
    time_delta = (SDL_GetTicks()/1000.0f) - time_delta;
}

