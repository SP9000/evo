#include "input.h"

static Uint8* keyState;
static int mouseX;
static int mouseY;

static TvInputDevice device;

int tv_input_init()
{
    /* TODO: load settings from file */

    device.joy = SDL_JoystickOpen(0);
    if(device.joy == NULL) {
        puts("No joysticks found");
        device.type = 0;
    }
    else {
        puts("Opened joystick for input");
        device.type = 1;
    }
    return 0;
}

void tv_input_quit()
{
    SDL_JoystickClose(device.joy);
}

void tv_input_update()
{
    SDL_GetMouseState(&mouseX, &mouseY);
    keyState = SDL_GetKeyState(NULL);
}

int tv_input_keydown(SDLKey key)
{
    return keyState[key];
}

int tv_input_mouse_x()
{
    return mouseX;
}

int tv_input_mouse_y()
{
    return mouseY;
}

