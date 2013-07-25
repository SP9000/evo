/*****************************************************************************/
/* input.h                                                                   */
/* This file defines functions for getting user input into the engine.       */
/* Note that while the word "mouse" is used often, it may refer to a virtual */
/* mouse for platforms with no physical mouse.                               */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: June 19, 2013                                                    */
/*****************************************************************************/
#ifndef _INPUT_H
#define _INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL/SDL.h>

#define INPUT_KEY_BACKSPACE     SDLK_BACKSPACE
#define INPUT_KEY_TAB           SDLK_TAB
#define INPUT_KEY_CLEAR         SDLK_CLEAR
#define INPUT_KEY_RETURN        SDLK_RETURN
#define INPUT_KEY_PAUSE         SDLK_PAUSE
#define INPUT_KEY_ESCAPE        SDLK_ESCAPE
#define INPUT_KEY_EXCLAIM       SDLK_EXCLAIM
#define INPUT_KEY_QUOTEDBL      SDLK_QUOTEDBL
#define INPUT_KEY_HASH          SDLK_HASH
#define INPUT_KEY_DOLLAR        SDLK_DOLLAR
#define INPUT_KEY_AMPERSAND     SDLK_AMPERSAND
#define INPUT_KEY_QUOTE         SDLK_QUOTE
#define INPUT_KEY_LEFTPAREN     SDLK_LEFTPAREN
#define INPUT_KEY_RIGHTPAREN    SDLK_RIGHTPAREN
#define INPUT_KEY_ASTERISK      SDLK_ASTERISK
#define INPUT_KEY_PLUS          SDLK_PLUS
#define INPUT_KEY_COMMA         SDLK_COMMA
#define INPUT_KEY_MINUS         SDLK_MINUS
#define INPUT_KEY_PERIOD        SDLK_PERIOD
#define INPUT_KEY_SLASH         SDLK_SLASH
#define INPUT_KEY_0             SDLK_0
#define INPUT_KEY_1             SDLK_1
#define INPUT_KEY_2             SDLK_2
#define INPUT_KEY_3             SDLK_3
#define INPUT_KEY_4             SDLK_4
#define INPUT_KEY_5             SDLK_6
#define INPUT_KEY_6             SDLK_7
#define INPUT_KEY_8             SDLK_8
#define INPUT_KEY_9             SDLK_9
#define INPUT_KEY_COLON         SDLK_COLON
#define INPUT_KEY_SEMICOLON     SDLK_SEMICOLON
#define INPUT_KEY_LESS          SDLK_LESS
#define INPUT_KEY_EQUALS        SDLK_EQUALS
#define INPUT_KEY_GREATER       SDLK_GREATER
#define INPUT_KEY_QUESTION      SDLK_QUESTION
#define INPUT_KEY_AT            SDLK_AT
#define INPUT_KEY_LEFTBRACKET   SDLK_LEFTBRACKET
#define INPUT_KEY_RIGHTBRACKET  SDLK_RIGHTBRACKET
#define INPUT_KEY_CARET         SDLK_CARET
#define INPUT_KEY_UNDERSCORE    SDLK_UNDERSCORE
#define INPUT_KEY_BACKQUOTE     SDLK_BACKQUOTE
#define INPUT_KEY_a             SDLK_a
#define INPUT_KEY_b             SDLK_b
#define INPUT_KEY_c             SDLK_c
#define INPUT_KEY_d             SDLK_d
#define INPUT_KEY_e             SDLK_e
#define INPUT_KEY_f             SDLK_f
#define INPUT_KEY_g             SDLK_g
#define INPUT_KEY_h             SDLK_h
#define INPUT_KEY_i             SDLK_i
#define INPUT_KEY_j             SDLK_j
#define INPUT_KEY_k             SDLK_k
#define INPUT_KEY_l             SDLK_l
#define INPUT_KEY_m             SDLK_m
#define INPUT_KEY_n             SDLK_n
#define INPUT_KEY_o             SDLK_o
#define INPUT_KEY_p             SDLK_p
#define INPUT_KEY_q             SDLK_q
#define INPUT_KEY_r             SDLK_r
#define INPUT_KEY_s             SDLK_s
#define INPUT_KEY_t             SDLK_t
#define INPUT_KEY_u             SDLK_u
#define INPUT_KEY_v             SDLK_v
#define INPUT_KEY_w             SDLK_w
#define INPUT_KEY_x             SDLK_x
#define INPUT_KEY_y             SDLK_y
#define INPUT_KEY_z             SDLK_z
#define INPUT_KEY_DELETE        SDLK_DELETE
#define INPUT_KEY_KP0           SDLK_KP0
#define INPUT_KEY_KP1           SDLK_KP1
#define INPUT_KEY_KP2           SDLK_KP2
#define INPUT_KEY_KP3           SDLK_KP3
#define INPUT_KEY_KP4           SDLK_KP4
#define INPUT_KEY_KP5           SDLK_KP5
#define INPUT_KEY_KP6           SDLK_KP6
#define INPUT_KEY_KP7           SDLK_KP7
#define INPUT_KEY_KP8           SDLK_KP8
#define INPUT_KEY_KP9           SDLK_KP9
#define INPUT_KEY_KP_PERIOD     SDLK_KP_PERIOD
#define INPUT_KEY_KP_DIVIDE     SDLK_KP_DIVIDE
#define INPUT_KEY_KP_MULTIPLY   SDLK_KP_MULTIPLY
#define INPUT_KEY_KP_MINUS      SDLK_KP_MINUS
#define INPUT_KEY_KP_PLUS       SDLK_KP_PLUS
#define INPUT_KEY_KP_ENTER      SDLK_KP_ENTER
#define INPUT_KEY_KP_EQUALS     SDLK_KP_EQUALS
#define INPUT_KEY_UP            SDLK_UP
#define INPUT_KEY_DOWN          SDLK_DOWN
#define INPUT_KEY_RIGHT         SDLK_RIGHT
#define INPUT_KEY_LEFT          SDLK_LEFT
#define INPUT_KEY_INSERT        SDLK_INSERT
#define INPUT_KEY_HOME          SDLK_HOME
#define INPUT_KEY_END           SDLK_END
#define INPUT_KEY_PAGEUP        SDLK_PAGEUP
#define INPUT_KEY_PAGEDOWN      SDLK_PAGEDOWN
#define INPUT_KEY_F1            SDLK_F1
#define INPUT_KEY_F2            SDLK_F2
#define INPUT_KEY_F3            SDLK_F3
#define INPUT_KEY_F4            SDLK_F4
#define INPUT_KEY_F5            SDLK_F5
#define INPUT_KEY_F6            SDLK_F6
#define INPUT_KEY_F7            SDLK_F7
#define INPUT_KEY_F8            SDLK_F8
#define INPUT_KEY_F9            SDLK_F9
#define INPUT_KEY_F10           SDLK_F10
#define INPUT_KEY_F11           SDLK_F11
#define INPUT_KEY_F12           SDLK_F12
#define INPUT_KEY_F13           SDLK_F13
#define INPUT_KEY_F14           SDLK_F14
#define INPUT_KEY_F15           SDLK_F15
#define INPUT_KEY_NUMLOCK       SDLK_NUMLOCK
#define INPUT_KEY_CAPSLOCK      SDLK_CAPSLOCK
#define INPUT_KEY_SCROLLOCK     SDLK_SCROLLOCK
#define INPUT_KEY_RSHIFT        SDLK_RSHIFT
#define INPUT_KEY_LSHIFT        SDLK_LSHIFT
#define INPUT_KEY_RCTRL         SDLK_RCTRL
#define INPUT_KEY_LCTRL         SDLK_LCTRL
#define INPUT_KEY_RALT          SDLK_RALT
#define INPUT_KEY_LALT          SDLK_LALT
#define INPUT_KEY_RMETA         SDLK_RMETA
#define INPUT_KEY_LSUPER        SDLK_LSUPER
#define INPUT_KEY_RSUPER        SDLK_RSUPER
#define INPUT_KEY_MODE          SDLK_MODE
#define INPUT_KEY_COMPOSE       SDLK_COMPOSE
#define INPUT_KEY_HELP          SDLK_HELP
#define INPUT_KEY_PRINT         SDLK_PRINT
#define INPUT_KEY_SYSREQ        SDLK_SYSREQ
#define INPUT_KEY_BREAK         SDLK_BREAK
#define INPUT_KEY_MENU          SDLK_MENU
#define INPUT_KEY_POWER         SDLK_POWER
#define INPUT_KEY_EURO          SDLK_EURO
#define INPUT_KEY_UNDO          SDLK_UNDO

/**
 * A simple union to abstract the buttons on the keyboard/joystick.
 */
typedef union tagInputButton {
    uint8_t key;
    uint8_t joy;
}TvInputButton;

/**
 * This structure represents the physical device used to communicate with the 
 * input system.
 */
typedef struct tagInputDevice {
    /* type of input device: 0=keyboard, 1=joystick */
    int type;
    /* if device is a joystick, this holds the reference to it */
    SDL_Joystick* joy;
}TvInputDevice;

/**
 * Initialize the input system. 
 */
int tv_input_init();

/**
 * Call upon exiting the client to cleanup input.
 */
void tv_input_quit();

/**
 * Calling this scans the input for the device. 
 * Call this every frame if you want input.
 */
void tv_input_update();

/**
 * Check if the given key is currently down.
 * @return zero if key is not down, nonzero if it is.
 */
int tv_input_keydown(SDLKey key);

/**
 * Get the position of the mouse in the X direction.
 * @return the current mouse X position.
 */
int tv_input_mouse_x();

/**
 * Get the position of the mouse in the Y direction.
 * @return the current mouse Y position.
 */
int tv_input_mouse_y();

#ifdef __cplusplus
}
#endif
#endif
