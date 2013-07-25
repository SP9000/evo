#include "timer.h"

float tv_time_delta;

int tv_time_init()
{
    tv_time_delta = 0.0f;
    return 0;
}

void tv_time_update()
{
    tv_time_delta = (SDL_GetTicks()/1000.0f) - tv_time_delta;
}

