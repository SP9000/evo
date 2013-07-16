/*****************************************************************************/
/* entity.h                                                                  */
/* This file defines the types and functions for managing "entities"         */
/* Entities are the basic objects within the game. Their behavior is defined */
/* by one or more "components". Each component within the entity is updated  */
/* with the client update.                                                   */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: April 2013                                                       */
/*****************************************************************************/
#ifndef _ENTITY_H
#define _ENTITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include "component.h"
#include "collision.h"
#include "draw.h"
#include "Gen/all.h"
#include "glib.h"
#include "scene.h"
#include "util.h"
#include "p99/p99_args.h"
#include "cJSON.h"

/**
 * The entity structure. Entities represent all objects in the engine.
 */
typedef struct Entity {
    int numChildren;
    int numComponents;
    GSList* children;
    GSList* components;
}Entity;

/**
 * Create an empty entity.
 * @return an empty entity.
 */
Entity* Entity_New();

/**
 * Add the given component to the given entity.
 * @param e the entity to add the component to.
 * @param c the component to add to the entity.
 */
void Entity_AddComponent(Entity* e, Component* c);

/**
 * Get the component of the specified type from the entity.
 * @param e the entity to retrieve the desired component from.
 * @param cid the ID of the type of the component to get.
 * @return the component if it exists in the entity, NULL otherwise.
 */
Component* Entity_GetComponent(Entity* e, int cid);

/**
 * This is called once all the components have been added to an entity.
 * It calls the start function of every component.
 * @param e the entity to start.
 */
void Entity_Start(Entity* e);

/**
 * Call the collide function on all components for the given entity.
 * @param e the entity to call the collide function for.
 * @param other the entity to pass as a parameter to the collide function.
 */
void Entity_Collide(Entity* e, Entity* other);

/**
 * Update all entities that have been created.
 */
void Entity_Update();


#ifdef __cplusplus
}
#endif
#endif

