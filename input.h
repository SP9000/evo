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
 * A simple union to abstract the buttons on the keyboard/joystick.
 */
typedef union tagInputButton {
    uint8_t key;
    uint8_t joy;
}InputButton;

/**
 * This structure represents the physical device used to communicate with the 
 * input system.
 */
typedef struct tagInputDevice {
    /* type of input device: 0=keyboard, 1=joystick */
    int type;
    /* if device is a joystick, this holds the reference to it */
    SDL_Joystick* joy;
}InputDevice;


/**
 * Initialize the input system. 
 */
int Input_Init();

/**
 * Call upon exiting the client to cleanup input.
 */
void Input_Quit();

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
