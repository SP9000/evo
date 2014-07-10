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

extern tvfloat tv_time_delta;

typedef struct tv_timer {
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
 * Update the given timer, and return the number of times it went off.
 */
tvuint tv_timer_update(tv_timer *timer);

#endif
