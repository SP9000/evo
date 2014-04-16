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

#include "types.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include "draw.h"
#include "scene.h"
#include "util.h"
#include "cJSON.h"
#include "component.h"

/**
 * The entity structure. Entities represent all objects in the engine.
 */
typedef struct TvEntity {
	TvVector3 pos;
	TvVector3 rot;
	TvVector3 scale;
	TvAABB aabb;

	/* the ID of the collider component (0 if none) attached to the entity */
	tvuint collide;

    tvuint numChildren;
    tvuint numComponents;
    struct TvEntity  *parent;
    TvArray /*TvEntity**/ *children;
	TvArray /*tv_component*/ *components;
}TvEntity;

/**
 * Initializes the entity system.
 * @return on success, 0, else an error code.
 */
int tv_entity_init();

/**
 * Create an empty entity.
 * @return an empty entity.
 */
TvEntity* tv_entity_new();

/**
 * Add the given component to the given entity.
 * @param e the entity to add the component to.
 * @param c the component to add to the entity.
 */
void tv_entity_add_component(TvEntity* e, tv_component* c);

/**
 * Add the given entity as a child to the given parent.
 * @param parent the entity to add the child to.
 * @param child the entity to add to the parent.
 */
void tv_entity_add_child(TvEntity* parent, TvEntity* child);

/**
 * Get the component of the specified type from the entity.
 * @param e the entity to retrieve the desired component from.
 * @param cid the ID of the type of the component to get.
 * @return the component if it exists in the entity, NULL otherwise.
 */
tv_component* tv_entity_get_component(TvEntity* e, tvuint cid);

/**
 * This is called once all the components have been added to an entity.
 * It calls the start function of every component.
 * @param e the entity to start.
 */
void tv_entity_start(TvEntity* e);

/**
 * Update all entities that have been created.
 */
void tv_entity_update();

/**
 * A macro to retrieve and cast the desired component from an entity.
 */
#define tv_entity_get(X, Y) ((Component_##Y *)Entity_GetComponent(X, CID_##Y))

#ifdef __cplusplus
}
#endif
#endif

