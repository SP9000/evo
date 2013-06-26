/******************************************************************************
 * entity.h
 * This file defines the types and functions for managing "entities" 
 * Entities are the basic objects within the game. Their behavior is defined by
 * one or more "components". Each component within the entity is updated with
 * the client update.
 *
 * Bryce Wilson
 * created: April 2013
 *****************************************************************************/
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
#include "stdcomponents.h"
#include "glib.h"
#include "util.h"

/**
 * A macro that builds an entity from the given components, but does not 
 *  initialize them.
 * This macro defines the following:
 *  a static inline Entity_New_XX function for allocating and returning an 
 *      entity containing the given components.
 *  The first parameter defines the name of the entity while remaining 
 *  parameters define the components of the entity.
 *  Usage:
 *      ENTITY(Bear,
 *          COMPONENT(Transform),
 *          COMPONENT(Growl),
 *          COMPONENT(Aggressive)
 *      )
 */
#define ENTITY(X, ...) \
        Entity* X = (Entity*)malloc(sizeof(Entity)); \
        X->numChildren = 0; \
        X->numComponents = 0; \
        Entity_AddComponent(X, Component_New_Transform()); \
        Entity_AddComponents(X, PP_NARG(__VA_ARGS__),  __VA_ARGS__); \
        Entity_Start(X); \
        Scene_Add(X); \

/**
 * A macro that defines a pre-built entity or "prefab".
 * This macro defines the following:
 *  a static inline Entity_New_XX function that allocates, initializes values
 *      for, and returns a prefab.
 *  Usage:
 *      ENTITY(Bear,
 *          COMPONENT_SET(Transform, {0.0f,0.0f,0.0f}),
 *
 */

/*
 * TODO:
#define PREFAB(X, ...) \
    static inline Enity* Entity_New_##X(Entity* e) { \
        e->children = NULL; \
        e->numChildren = 0; \
        e->numComponents = 0; \
        Prefab_AddComponents(e, PP_NARG(__VA_ARGS__), __VA_ARGS__); \
        Prefab_InitComponents(e, PP_NARG(__VA_ARGS__), __VA_ARGS__); \
    }
*/

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
 * Add the variable number of components to the given entity.
 * @param e the entity to add the components to.
 * @param numComponents the number of components to add to the entity.
 * @param ... the variable arguments to add to the entity.
 */
void Entity_AddComponents(Entity* e, int numComponents, ...);

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

#ifdef __cplusplus
}
#endif
#endif

