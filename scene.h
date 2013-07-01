/*****************************************************************************/
/* scene.h                                                                   */
/* This file contains definitions for functions to manage the "scene" in the */
/* engine. The scene is a spatial representation of the objects to be        */
/* rendered.                                                                 */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: June 18, 2013                                                    */
/*****************************************************************************/
#ifndef _SCENE_H
#define _SCENE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "glib.h"
#include "model.h"
#include "stdcomponents.h"

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
void Scene_Add(Component_Model* m);

/**
 * Add the given model to the GUI portion of the display. 
 * The GUI is described in two-dimensions as opposed to the (typically) 3D
 * scene. The GUI is generally rendered last. 
 * @param 
 */
void Scene_AddGUI(Component_Model* m);

/**
 * Run the given function with the given user data over all entities in the
 * scene.
 * @param func the function to run on all objects in the scene.
 * @param id if non-zero, only call func for entities that have a component
 *  with this ID.
 */
void Scene_Foreach(void (*func)(Component_Model*), int cid);

#ifdef __cplusplus
}
#endif
#endif

