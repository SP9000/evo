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

#include "glib.h"
#include "Gen/all.h"

extern GSList* scene;

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
 * Add the given widget to the GUI portion of the display. 
 * The GUI is described in two-dimensions as opposed to the (typically) 3D
 * scene. It is drawn ontop of the scene. 
 * @param w The widget to add to the GUI scene.
 */
void Scene_AddWidget(Component_Widget* w);

#ifdef __cplusplus
}
#endif
#endif

