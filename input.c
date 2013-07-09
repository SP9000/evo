#include "input.h"

static Uint8* keyState;
static int mouseX;
static int mouseY;

static InputDevice device;

int Input_Init()
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

void Input_Quit()
{
    SDL_JoystickClose(device.joy);
}

void Input_Update()
{
    SDL_GetMouseState(&mouseX, &mouseY);
    keyState = SDL_GetKeyState(NULL);
}

int Input_KeyDown(SDLKey key)
{
    return keyState[key];
}

int Input_MouseX()
{
    return mouseX;
}

int Input_MouseY()
{
    return mouseY;
}

