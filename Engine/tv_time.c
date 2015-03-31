#include "tv_time.h"

tvfloat tv_time_delta;	/* the number of milliseconds since the last update */
static tvbool paused;	/* if true, the delta time is not updated */

/*******************************************************************************
 * tv_time_init
 * Initializes the delta time value.
*******************************************************************************/
int tv_time_init()
{
    tv_time_delta = 0;
    return 0;
}
/*******************************************************************************
 * tv_time_update
 * Updates the engine time to time since the engine was invoked and updates the
 * engine time delta to the difference of the current time and the time from
 * the last time update.
*******************************************************************************/
void tv_time_update()
{
	static tvfloat last_update = 0;
	if(!paused) {
		tv_time_delta = (SDL_GetTicks()/1000.0f) - last_update;
	}
	last_update = SDL_GetTicks()/1000.0f;
}
/*******************************************************************************
 * tv_timer_update
 * TODO:
*******************************************************************************/
tvuint tv_timer_update(tv_timer *timer)
{
	tvuint times_triggererd = 0;
	tvuint i;
	timer->current += tv_time_delta * timer->count_factor;

	/* is this timer just being used to keep time (no triggers)? */
	if(timer->max_rollover == TV_TIMER_NO_ROLLOVER || timer->set == TV_TIMER_NO_RESET) {
		/* yes */
		return 0;
	}

	/* TODO: only triggers max 1 times.  didn't like how I did this before.
	 * but this is...obviously not a permanent "solution". */
	if(timer->current > timer->set) {
		timer->current -= timer->set;
		return 1;
	}
	else if(timer->current < 0.0f) {
		timer->current += timer->set;
		return 1;
	}
	return times_triggererd;
}
/*******************************************************************************
 * tv_timer_new
 * Allocates/initalizes a new timer.
*******************************************************************************/
tv_timer *tv_timer_new(tvfloat count_factor, tvuint max_rollovers, tvfloat reset_value)
{
	tv_timer *t = (tv_timer*)tv_alloc(sizeof(tv_timer));
	t->count_factor = count_factor;
	/* set the current value to 0 if counting up or the reset value if down. */
	if(count_factor < 0.0f) {
		t->current = 0.0f;
	}
	else {
		t->current = reset_value;
	}
	t->max_rollover = max_rollovers;
	t->set = reset_value;
}
/*******************************************************************************
 * tv_timer_reset
 * Resets a timer to its reset value(?)
 * TODO: currently resets timer to 0.
*******************************************************************************/
void tv_timer_reset(tv_timer *timer)
{
	timer->current = 0.0f;
}
/*******************************************************************************
 * tv_time_pause
 * Sets the paused flag so that the delta time is not updated.
*******************************************************************************/
void tv_time_pause()
{
	paused = TRUE;
	tv_time_delta = 0;
}
/*******************************************************************************
 * tv_time_unpause
 * Clears the paused flag so- delta time will be updated each call to 
 * tv_time_update.
*******************************************************************************/
void tv_time_unpause()
{
	paused = FALSE;
}

