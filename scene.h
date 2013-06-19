/******************************************************************************
 * scene.h
 * This file contains definitions for functions to manage the "scene" in the
 * engine. The scene is a spatial representation of the objects to be rendered.
 *
 * Bryce Wilson
 * created: June 18, 2013
 *****************************************************************************/
#ifndef _SCENE_H
#define _SCENE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "glib.h"
#include "model.h"

/**
 * Initialize the scene system.
 * @return zero on success, nonzero on failure.
 */
int Scene_Init();

/**
 * Destroy all items in the scene and clean up.
 * @return zero on success, nonzero on failure.
 */
int Scene_Quit();

/**
 * Add the given model to the scene.
 * @param m the model to add to the scene.
 */
void Scene_Add(Model* m);

/**
 * Run the given function with the given user data over all models in the
 * scene.
 * @param func the function to run on all objects in the scene.
 */
void Scene_Foreach(void (*func)(Model*));

#ifdef __cplusplus
}
#endif
#endif
