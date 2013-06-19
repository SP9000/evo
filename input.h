/******************************************************************************
 * input.h
 * This file defines functions for getting user input into the engine.
 * Note that while the word "mouse" is used often, it may refer to a virtual 
 * mouse for platforms with no physical mouse.
 *
 * Bryce Wilson
 * created: June 19, 2013
 *
 * TODO: don't use SDL for the key names.
 *****************************************************************************/
#ifndef _INPUT_H
#define _INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL/SDL.h>

/**
 * Calling this scans the input for the device. 
 * Call this every frame if you want input.
 */
void Input_Update();

/**
 * Check if the given key is currently down.
 * @return zero if key is not down, nonzero if it is.
 */
int Input_KeyDown(SDLKey key);

/**
 * Get the position of the mouse in the X direction.
 * @return the current mouse X position.
 */
int Input_MouseX();

/**
 * Get the position of the mouse in the Y direction.
 * @return the current mouse Y position.
 */
int Input_MouseY();

#ifdef __cplusplus
}
#endif
#endif
