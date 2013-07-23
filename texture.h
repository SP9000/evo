/*****************************************************************************/
/* texture.h                                                                 */
/* this file and the corresponding c file contain functions to load textures */
/* from file and store relevant data to render them.                         */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June, 2013                                                       */
/*****************************************************************************/
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <SDL/SDL.h>
#include <GL/glew.h>
#include "glib.h"

typedef struct tagTexture {
    unsigned width;
    unsigned height;
    GLuint id;
}Texture;

/**
 * Initialize the texture system.
 * Call this before using any texture functions.
 * @return 0 if successful, else nonzero.
 */
int Texture_Init();

/**
 * Cleanup the texture system by freeing all textures created.
 */
void Texture_Quit();

/**
 * Load the given bitmap file into a texture and return it.
 * @param file the bitmap file to load into a texture.
 * @return the texture created from the given file.
 */
Texture Texture_LoadBMP(char* file);

#endif
