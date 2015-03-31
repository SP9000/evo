/*****************************************************************************/
/* timer.h                                                                   */
/* This file contains functions for dealing with time in the engine.         */
/* Of particular note is the time_delta variable, updated in Time_Update.    */
/* Pretty much any movement or time-sensitive operation should use this value*/
/* as a scalar to get movement independent of the framerate.                 */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created July 8, 2013                                                      */
/*****************************************************************************/
#ifndef TIMER_H
#define TIMER_H

#include <SDL/SDL.h>
#include "tv_types.h"
#include "tv_alloc.h"

#define TV_TIMER_NO_ROLLOVER 0
#define TV_TIMER_NO_RESET 0

extern tvfloat tv_time_delta;

typedef struct tv_timer {
	tvfloat count_factor;
	tvfloat current;
	tvfloat set;
	tvuint max_rollover;
}tv_timer;

/**
 * Initialize the main timers for the engine.
 */
int tv_time_init();

/**
 * Update the engines' timers.
 */
void tv_time_update();

/**
 * Pause the engine. 
 * Until unpaused, delta_time will be 0.
 */
void tv_time_pause();

/**
 * Unpause the engine. 
 * Delta_time will contain the difference between 2 updates. 
 * By default the engine is unpaused.
 */
void tv_time_unpause();


/**
 * Create and initialize a timer.
 * @param count_factor the amount to scale the time value by when updating
 *  the timer.  (use 1.0f for typical up-counter or -1.0f for down).
 * @param max_rollovers the number of times the timer may "trigger" in one
 *  update. 1 is generally suitable for most simply, if-else applications.
 *   if you're only using the timer to keep a time use TV_TIMER_NO_ROLLOVER.
 * @param reset_value the amount of time between triggers of this timer.
 *   If you're not using triggers, use TV_TIMER_NO_RESET.
 */
tv_timer *tv_timer_new(tvfloat count_factor, tvuint max_rollovers, tvfloat reset_value);

/** 
 * Reset the given timer to 0.
 * @param timer the timer to reset.
 */
void tv_timer_reset(tv_timer *timer);

/** 
 * Get the time from the given timer.
 * @param timer the timer to check the time of.
 * @return the given timer's current time.
 */
static float tv_timer_get_time(tv_timer *timer) {return timer->current;}

/** 
 * Update the given timer, and return the number of times it went off.
 */
tvuint tv_timer_update(tv_timer *timer);

#endif
