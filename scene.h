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
#include "Gen/all.h"

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
 * Add the given renderer to the scene.
 * @param r the renderer to add to the scene.
 */
void tv_scene_add(Component_Renderer* r);

#ifdef __cplusplus
}
#endif
#endif

