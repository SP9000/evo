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
#include "glib.h"
#include "matrix.h"
#include "scene.h"
#include "types.h"
#include "Gen/all.h"

extern Component_Camera* main_cam;

typedef struct tagDrawTarget {
    /* the framebuffer ID of the target. */
    GLuint fbID;
    /* the ID of the texture to render to (if any). */
    GLuint texID;
    /* renderbuffer depth buffer handle */
    GLuint depthID;
}DrawTarget;

typedef struct tagMap {
    guint32 width, height;
}Map;

/* expose the screen surface. h8r's gon' h8 */
extern SDL_Surface* screen;

/**
 * Initialize the libraries needed for rendering. 
 * @return the success of the initialization - 0 on success, !0 on error.
 */
int Draw_Init();


/**
 * Clean up all libraries and allocations of the rendering system.
 */
void Draw_Quit();

/**
  * Clear the screen and do anything else necessary before beginning 
  * to render a frame.
  */
void Draw_StartFrame();

/**
 * Do the post processing on the scene and do the final render to the back 
 * buffer.
 */
void Draw_FinishFrame();

/**
 * Draw all models in the scene.
 */
void Draw_Scene();

/**
 * Generate a new target to draw to.
 * @return the new target.
 */
DrawTarget* Draw_NewTarget();

/**
 * Sets the target of the current render.
 * @param target the target to render to. NULL = back buffer.
 */
void Draw_SetTarget(DrawTarget* target);

/**
 * Returns the given DrawTarget as a texture.
 * @param target the target to get the texture of.
 * @return a texture usable with Material_SetTexture.
 */
Texture Draw_TargetToTexture(DrawTarget* target);

/**
 * Draw GUI ontop of the currently rendered scene.
 * Draw the root widget and all children of it ontop of whatever was last 
 * rendered.
 */
void Draw_GUI();

/**
 * Draw the given model.
 * Draws the model m, buffering its data to the GPU if this is the first time
 * that the model has been rendered.
 * @param m the model to draw.
 */
void Draw_Model(Component_Model* m);

/**
 * Draw the given widget.
 * @param w the widget to draw.
 */
void Draw_Widget(Component_Widget* w);

/**
 * Move the camera by the given X,Y,Z amounts.
 * @param x the amount to move the camera in the X direction.
 * @param y the amount to move the camera in the Y direction.
 * @param z the amount to move the camera in the Z direction.
 */
void Draw_MoveCamera(float x, float y, float z);

/**
 * Resize the display to the given width and height.
 * This function notifies all cameras of the change so they can update their
 * aspect ratios and recalculate their projection matricies as well.
 * TODO: no it doesn't.
 * @param w the new width of the screen.
 * @param h the new height of the screen.
 */
void Draw_ResizeScreen(int w, int h);

/**
 * Add the given camera to the list of cameras.
 * @param cam the camera to be added and used henceforth for rendering. 
 */
void Draw_AddCamera(Component_Camera* cam);

#ifdef __cplusplus
}
#endif

#endif
