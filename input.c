#include "input.h"

static tv_input_button_state keyboard_state[TV_INPUT_KEYBOARD_MAX_BUTTONS];
static tv_input_button_state mouse_button_states[TV_INPUT_MOUSE_MAX_BUTTONS];
static tv_input_button_state joypad_button_states[TV_INPUT_JOYPAD_MAX_BUTTONS];
static int mouseX;
static int mouseY;

static TvInputDevice device;

int tv_input_init()
{
	tvuint i;

    /* TODO: load settings from file */
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

void tv_input_quit()
{
    SDL_JoystickClose(device.joy);
}

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

int tv_input_keydown(SDLKey key)
{
    //return keyboard_state[key];
	return 0;
}

tv_vector2 tv_input_mouse_pos()
{
	tv_vector2 ret = {mouseX, mouseY};
	return ret;
}
int tv_input_mouse_x()
{
    return mouseX;
}

int tv_input_mouse_y()
{
    return mouseY;
}

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