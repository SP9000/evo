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
#include "tv_types.h"
#include "tv_alloc.h"

typedef struct tagTvTexture {
	tvchar* name;
    tvuint width;
    tvuint height;
    GLuint id;
	TvHashHandle hh;
}TvTexture;

/**
 * Initialize the texture system.
 * Call this before using any texture functions.
 * @return 0 if successful, else nonzero.
 */
int tv_texture_init();

TvTexture *tv_texture_new();

/**
 * Cleanup the texture system by freeing all textures created.
 */
void tv_texture_quit();

/**
 * Load the given bitmap file into a texture and return it.
 * @param file the bitmap file to load into a texture.
 * @return the texture created from the given file.
 */
TvTexture* tv_texture_load_bmp(char* file);

#endif