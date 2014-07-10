/*****************************************************************************/
/* draw.h                                                                    */
/* This file defines all functions used for rendering within the engine.     */
/* In theory, this code contains the majority of the platform dependent code */
/* in the engine though you'll find some in material.h as well.              */
/* The hardware acclerated version should be compatible with OpenGL 2.0 and  */
/* conform at least up to 4.0  standard.                                     */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: April, 2013                                                      */
/*****************************************************************************/
#ifndef DRAW_H
#define DRAW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "camera.h"
#include "material.h"
#include "tv_matrix.h"
#include "model.h"
#include "tv_scene.h"
#include "tv_texture.h"
#include "tv_types.h"
#include "tv_stats.h"

extern tv_camera* main_cam;

typedef struct tagTvDrawTarget {
    /* the framebuffer ID of the target. */
    GLuint fbID;
    /* the ID of the texture to render to (if any). */
    GLuint texID;
    /* renderbuffer depth buffer handle */
    GLuint depthID;
}TvDrawTarget;

typedef struct tagMap {
    tvuint width, height;
}Map;

/* expose the screen surface. h8r's gon' h8 */
extern SDL_Surface* screen;

/* the global GUI camera */
extern tv_camera *tv_camera_gui;

/**
 * Initialize the libraries needed for rendering. 
 * @return the success of the initialization - 0 on success, !0 on error.
 */
int tv_draw_init();


/**
 * Clean up all libraries and allocations of the rendering system.
 */
void tv_draw_quit();

/**
  * Clear the screen and do anything else necessary before beginning 
  * to render a frame.
  */
void tv_draw_start_frame();

/**
 * Do the post processing on the scene and do the final render to the back 
 * buffer.
 */
void tv_draw_finish_frame();

/**
 * Draw all models in the scene.
 */
void tv_draw_scene();

/**
 * Generate a new target to draw to.
 * @return the new target.
 */
TvDrawTarget* tv_draw_new_target();

/**
 * Sets the target of the current render.
 * @param target the target to render to. NULL = back buffer.
 */
void tv_draw_set_target(TvDrawTarget* target);

/**
 * Returns the given DrawTarget as a texture.
 * @param target the target to get the texture of.
 * @return a texture usable with Material_SetTexture.
 */
TvTexture tv_draw_target_to_texture(TvDrawTarget* target);

/**
 * Draw GUI ontop of the currently rendered scene.
 * Draw the root widget and all children of it ontop of whatever was last 
 * rendered.
 */
void tv_draw_gui();

/**
 * Resize the display to the given width and height.
 * This function notifies all cameras of the change so they can update their
 * aspect ratios and recalculate their projection matricies as well.
 * TODO: no it doesn't.
 * @param w the new width of the screen.
 * @param h the new height of the screen.
 */
void tv_draw_resize_screen(int w, int h);

/**
 * Draw the given texture within the bounds of the given rect.
 * @param tex the texture to draw.
 * @param rect the rectangle to draw the texture within.
 */
void tv_draw_texture(TvTexture tex, tv_rect* rect);

/**
 * OpenGL wrappers
 */
void tv_draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void tv_draw_arrays(GLenum mode, GLint first, GLsizei count);
void tv_delete_buffers(GLsizei n, const GLuint * buffers);


#ifdef __cplusplus
}
#endif
#endif