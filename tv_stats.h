/*****************************************************************************/
/* tv_stats.h                                                                */
/* This file contains definitions for functions that involve gathering       */
/* information about the state of the engine (FPS, amount rendered, etc.)    */
/* Bryce Wilson                                                              */
/* Created April 29 2014                                                     */
/*****************************************************************************/
#ifndef TV_STATS_H
#define TV_STATS_H
#ifdef __cplusplus
extern "C" {
#endif

#define TV_STATS_FRAME_APPROXIMATION_SAMPLES 60

#include "types.h"
#include "timer.h"

/**
 * Returns the current approximate frames per second of the engine.
 * @return An approximation of the current frames per second.
 */
tvfloat tv_stats_fps();

/**
 * Returns the number of primitives that have been rendererd.
 * @return the number of primitives rendererd since the last call to this 
 *   function.
 */
tvuint tv_stats_primitives_rendered();

/**
 * This should be called each frame to keep accurate statistics of the engine. 
 */
void tv_stats_update();


/**
 * Notify the statistics system that more primitives have been rendered.
 */
void tv_stats_notify_primitives_rendered(tvuint count);

#ifdef __cplusplus
}
#endif
#endif