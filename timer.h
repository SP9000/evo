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
#include "types.h"

extern tvfloat tv_time_delta;

/**
 * Initialize the main timers for the engine.
 */
int tv_time_init();

/**
 * Update the engines' timers.
 */
void tv_time_update();

#endif
