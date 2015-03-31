#include "tv_input.h"
#include "tv_draw.h"	//for screen width and height

/*****************************************************************************/
static tv_input_button_state keyboard_state[TV_INPUT_KEYBOARD_MAX_BUTTONS];
static tv_input_button_state mouse_button_states[TV_INPUT_MOUSE_MAX_BUTTONS];
static tv_input_button_state joypad_button_states[TV_INPUT_JOYPAD_MAX_BUTTONS];
static int mouseX;
static int mouseY;
static TvInputDevice device;

/******************************************************************************
 * tv_input_init
 * Opens joysticks if possible and otherwise prepares the engine for accepting
 * input.
 * TODO: load settings from file 
 *****************************************************************************/
int tv_input_init()
{
	tvuint i;
    device.joy = SDL_JoystickOpen(0);
    if(device.joy == NULL) {
        puts("No joysticks found");
    }
    else {
        puts("Opened joystick for input");
    }
	for(i = 0; i < TV_INPUT_MOUSE_MAX_BUTTONS; ++i) {
		mouse_button_states[i] = TV_INPUT_BUTTON_NULL;
	}
	for(i = 0; i < TV_INPUT_JOYPAD_MAX_BUTTONS; ++i) {
		joypad_button_states[i] = TV_INPUT_BUTTON_NULL;
	}
	for(i = 0; i < TV_INPUT_KEYBOARD_MAX_BUTTONS; ++i) {
		keyboard_state[i] = TV_INPUT_BUTTON_NULL;
	}
    return 0;
}
/******************************************************************************
 * tv_input_quit
 * Closes any open joysticks.
 *****************************************************************************/
void tv_input_quit()
{
    SDL_JoystickClose(device.joy);
}
/******************************************************************************
 * tv_input_update
 * Polls any input devices that are configured to be used.
 *****************************************************************************/
void tv_input_update()
{
	tvuint i;
    SDL_GetMouseState(&mouseX, &mouseY);

	for(i = 0; i < TV_INPUT_MOUSE_MAX_BUTTONS; ++i) {
		if(mouse_button_states[i] == TV_INPUT_BUTTON_RELEASED) {
			mouse_button_states[i] = TV_INPUT_BUTTON_UP;
		}
		else if(mouse_button_states[i] == TV_INPUT_BUTTON_PRESSED) {
			mouse_button_states[i] = TV_INPUT_BUTTON_DOWN;
		}	
	}
	for(i = 0; i < TV_INPUT_JOYPAD_MAX_BUTTONS; ++i) {
		if(joypad_button_states[i] == TV_INPUT_BUTTON_RELEASED) {
			joypad_button_states[i] = TV_INPUT_BUTTON_UP;
		}
		else if(joypad_button_states[i] == TV_INPUT_BUTTON_PRESSED) {
			joypad_button_states[i] = TV_INPUT_BUTTON_DOWN;
		}	
	}
	for(i = 0; i < TV_INPUT_KEYBOARD_MAX_BUTTONS; ++i) {
		if(keyboard_state[i] == TV_INPUT_BUTTON_RELEASED) {
			keyboard_state[i] = TV_INPUT_BUTTON_UP;
		}
		else if(keyboard_state[i] == TV_INPUT_BUTTON_PRESSED) {
			keyboard_state[i] = TV_INPUT_BUTTON_DOWN;
		}	
	}
}
/******************************************************************************
 * tv_input_button_new
 * Create and return a button structure with the given parameter attributes.
 *****************************************************************************/
tv_input_button tv_input_button_new(tv_input_device_type type, tvuint button)
{
	tv_input_button b = {type, button};
	return b;
}
/******************************************************************************
 * tv_input_keydown
 * TODO:
 *****************************************************************************/
int tv_input_keydown(SDLKey key)
{
    /* return keyboard_state[key]; */
	return 0;
}
/******************************************************************************
 * tv_input_mouse_pos
 * returns the last read mouse coordinates.
 *****************************************************************************/
tv_vector2 tv_input_mouse_pos()
{
	tv_vector2 ret = {(tvfloat)mouseX, (tvfloat)mouseY};
	return ret;
}
/******************************************************************************
 * tv_input_mouse_x
 * returns the last read mouse x-coordinate.
 *****************************************************************************/
int tv_input_mouse_x()
{
    return mouseX;
}
/******************************************************************************
 * tv_input_mouse_y
 * returns the last read mouse y-coordinate.
 *****************************************************************************/
int tv_input_mouse_y()
{
    return mouseY;
}
/******************************************************************************
 * tv_input_mouse_y
 * returns the last read mouse y-coordinate in the range [0-1]
 *****************************************************************************/
tvfloat tv_input_mouse_y_normalized()
{
	return (tvfloat)mouseY / (tvfloat)tv_draw_screen_dim().y;
}
/******************************************************************************
 * tv_input_mouse_y
 * returns the last read mouse x-coordinate in the range [0-1]
 *****************************************************************************/
tvfloat tv_input_mouse_x_normalized()
{
	return (tvfloat)mouseX / (tvfloat)tv_draw_screen_dim().x;
}
/******************************************************************************
 * tv_input_buttondown
 * Returns the state of the given button as last polled- TRUE=down.
 * Down means that the button was up, but is now down.
 *****************************************************************************/
tvbool tv_input_buttondown(tv_input_button button)
{
	if(button.type == TV_INPUT_KEYBOARD) {
		if(keyboard_state[button.button] == TV_INPUT_BUTTON_DOWN || keyboard_state[button.button] == TV_INPUT_BUTTON_PRESSED) {
			return TRUE;
		}
	}
	else if(button.type == TV_INPUT_JOYPAD) {
		if(joypad_button_states[button.button] == TV_INPUT_BUTTON_DOWN || joypad_button_states[button.button] == TV_INPUT_BUTTON_PRESSED) {
			return TRUE;
		}
	}
	else if(button.type == TV_INPUT_MOUSE) {
		if(mouse_button_states[button.button] == TV_INPUT_BUTTON_DOWN || mouse_button_states[button.button] == TV_INPUT_BUTTON_PRESSED) {
			return TRUE;
		}
	}
	return FALSE;
}
/******************************************************************************
 * tv_input_buttonpressed
 * Returns the state of the given button as last polled- TRUE=pressed.
 * Pressed means the button is down (it is not concerned with any past state).
 *****************************************************************************/
tvbool tv_input_buttonpressed(tv_input_button button)
{
	if(button.type == TV_INPUT_JOYPAD) {
		if(keyboard_state[button.button] == TV_INPUT_BUTTON_PRESSED) {
			return TRUE;
		}
	}
	else if(button.type == TV_INPUT_KEYBOARD) {
		if(joypad_button_states[button.button] == TV_INPUT_BUTTON_PRESSED) {
			return TRUE;
		}
	}
	else if(button.type == TV_INPUT_MOUSE) {
		if(mouse_button_states[button.button] == TV_INPUT_BUTTON_PRESSED) {
			return TRUE;
		}
	}
	return FALSE;
}
/******************************************************************************
 * tv_input_buttonreleased
 * Returns the state of the given button as last polled- TRUE=released.
 * Released means the button is now up, but was down last frame. 
 *****************************************************************************/
tvbool tv_input_buttonreleased(tv_input_button button)
{
	if(button.type == TV_INPUT_JOYPAD) {
		if(keyboard_state[button.button] == TV_INPUT_BUTTON_RELEASED) {
			return TRUE;
		}
	}
	else if(button.type == TV_INPUT_KEYBOARD) {
		if(joypad_button_states[button.button] == TV_INPUT_BUTTON_RELEASED) {
			return TRUE;
		}
	}
	else if(button.type == TV_INPUT_MOUSE) {
		if(mouse_button_states[button.button] == TV_INPUT_BUTTON_RELEASED) {
			return TRUE;
		}
	}
	return FALSE;
}
/******************************************************************************
 * tv_input_buttonreleased
 * Returns the state of the given button as last polled- TRUE=up.
 * Up means the button is not pressed (regardless of past state).
 *****************************************************************************/
tvbool tv_input_buttonup(tv_input_button button)
{
	if(button.type == TV_INPUT_JOYPAD) {
		if(keyboard_state[button.button] == TV_INPUT_BUTTON_UP) {
			return TRUE;
		}
	}
	else if(button.type == TV_INPUT_KEYBOARD) {
		if(joypad_button_states[button.button] == TV_INPUT_BUTTON_UP) {
			return TRUE;
		}
	}
	else if(button.type == TV_INPUT_MOUSE) {
		if(mouse_button_states[button.button] == TV_INPUT_BUTTON_UP) {
			return TRUE;
		}
	}
	return FALSE;
}
/******************************************************************************
 * tv_input_register_button_event
 * TODO:
 *****************************************************************************/
void tv_input_register_button_event(tv_input_device_type type, tvuint id, tvbool pressed)
{
	switch(type) {
	case TV_INPUT_JOYPAD:
		if(pressed) {
			joypad_button_states[id] = TV_INPUT_BUTTON_PRESSED;
		}
		else {
			joypad_button_states[id] = TV_INPUT_BUTTON_RELEASED;
		}
		break;
	case TV_INPUT_KEYBOARD:
		if(pressed) {
			keyboard_state[id] = TV_INPUT_BUTTON_PRESSED;
		}
		else {
			keyboard_state[id] = TV_INPUT_BUTTON_RELEASED;
		}
		break;
	case TV_INPUT_MOUSE:
		if(pressed) {
			mouse_button_states[id] = TV_INPUT_BUTTON_PRESSED;
		}
		else {
			/* TODO: not very clean. */
			if(id == TV_INPUT_MOUSE_WHEELDOWN || id == TV_INPUT_MOUSE_WHEELUP) {
				return;
			}
			mouse_button_states[id] = TV_INPUT_BUTTON_RELEASED;
		}
		break;
	}
	//		return SDL_JoystickGetButton(device.joy, button.button);
}