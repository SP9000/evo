#include "tv_stats.h"

static tvuint num_rendered_primitives = 0;

tvfloat tv_stats_fps()
{
	static tvfloat update_times[TV_STATS_FRAME_APPROXIMATION_SAMPLES];
	static tvfloat update_sum = 0.0f;
	static tvuint index = 0;
	
	update_sum -= update_times[index];
	update_sum += tv_time_delta;
	update_times[index] = tv_time_delta;
	if(++index == TV_STATS_FRAME_APPROXIMATION_SAMPLES) {
		index = 0;
	}
	return 1.0f/(update_sum/((tvfloat)TV_STATS_FRAME_APPROXIMATION_SAMPLES));
}

tvuint tv_stats_primitives_rendered()
{
	tvuint count = num_rendered_primitives;
	num_rendered_primitives = 0;
	return count;
}
void tv_stats_notify_primitives_rendered(tvuint count)
{
	num_rendered_primitives += count;
}

void tv_stats_update()
{

}