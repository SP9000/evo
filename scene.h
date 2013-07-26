/*****************************************************************************/
/* scene.h                                                                   */
/* This file contains definitions for functions to manage the "scene" in the */
/* engine. The scene is a spatial representation of the objects to be        */
/* rendered. When a Model or Widget component is added to the engine, it gets*/
/* added automatically to the scene system.                                  */
/* Bryce Wilson                                                              */
/* created: June 18, 2013                                                    */
/*****************************************************************************/
#ifndef _SCENE_H
#define _SCENE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "component.h"

extern GSList* scene_layers[32];

/**
 * Initialize the scene system.
 * @return zero on success, nonzero on failure.
 */
int tv_scene_init();

/**
 * Destroy all items in the scene and clean up.
 * @return zero on success, nonzero on failure.
 */
int tv_scene_quit();

/**
 * Associate the given ID with a given render function and render layer.
 * @param render_func the function to use to render components of the given id.
 * @param layer the layer on which components of this id should be rendered.
 * @param id the id to associate with all that monkey business.
 */
void tv_scene_register_renderer(void (*render_func)(TvComponent*), tvuint layer, tvuint id);


/**
 * Add a component of an ID previously registered with scene_register_renderer
 * to the scene.
 * @param c the renderer component to add.
 */
void tv_scene_add(TvComponent* c);

#ifdef __cplusplus
}
#endif
#endif

