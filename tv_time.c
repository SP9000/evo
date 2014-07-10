#include "tv_time.h"

tvfloat tv_time_delta;

int tv_time_init()
{
    tv_time_delta = 0;
    return 0;
}

void tv_time_update()
{
	static tvfloat last_update = 0;
    tv_time_delta = (SDL_GetTicks()/1000.0f) - last_update;
	last_update = SDL_GetTicks()/1000.0f;
}

tvuint tv_timer_update(tv_timer *timer)
{
	tvuint times_triggererd = 0;
	tvuint i;
	timer->current -= tv_time_delta;
	for(i = 0; i < timer->max_rollover; ++i, ++times_triggererd) {
		if(timer->current < 0.0f) {
			timer->current += timer->set;
		}
		else {
			break;
		}
	}
	return times_triggererd;
}
