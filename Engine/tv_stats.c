#include "tv_stats.h"

/*****************************************************************************/
static tvuint num_rendered_primitives = 0;

/******************************************************************************
 * tv_stats_fps()
 * Averages the last TV_STATS_FRAME_APPROXIMATION_SAMPLES of framerate data
 * and returns an estimate of the current FPS based on that average.
******************************************************************************/
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
/******************************************************************************
 * tv_stats_primitives_renderered
 * Returns the number of primitives that have been rendered via tv_draw_arrays
 * and tv_draw_elements calls.  Primitives rendered directly via OpenGL 
 * (e.g. glDrawArrays) or other rendering API's will not be counted.
 * The value returned is the number of primitves rendered since the last call
 * to this function.
 * TODO: should be per frame?
******************************************************************************/
tvuint tv_stats_primitives_rendered()
{
	tvuint count = num_rendered_primitives;
	num_rendered_primitives = 0;
	return count;
}
/******************************************************************************
 * tv_stats_notify_primitives_rendered
 * Called by rendering functions to update the primitives rendered statistics.
******************************************************************************/
void tv_stats_notify_primitives_rendered(tvuint count)
{
	num_rendered_primitives += count;
}
/******************************************************************************
 * tv_stats_update
 * Does nothing.
******************************************************************************/
void tv_stats_update()
{

}