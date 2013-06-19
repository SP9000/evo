#include "input.h"

static Uint8 keyState[256];
static int mouseX;
static int mouseY;


void Input_Update()
{
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_GetKeyState(keyState);
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

