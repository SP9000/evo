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
#include "glib.h"
#include "util.h"

#define ATTRIBUTE(X) X;
#define COMPONENT(X) X
#define COMPONENT_SET(X, Y) X, (Y)Y

/** 
 * A macro that defines basic functions for every component.
 * This macro defines the following:
 *  a static Start() function - called to initialize the component.
 *  a static Update() function - called per frame to update the component.
 *  a static inline Component_New_XX() function that allocates and returns the
 *      component XX.
 *  Note that because Start() and Update() are static, you should make sure
 *  components are only included in one compilation unit to avoid redundant 
 *  code generation. 
 *  To define members of a comonent, use the ATTRIBUTE macro. 
 *  Here is an example of this very unintuitive process:
 *  DEFINE_COMPONENT(
 *      Transform,
 *      ATTRIBUTE(Vector3 position)
 *      ATTRIBUTE(Vector3 rotation)
 *      ATTRIBUTE(Vector3 scale)
 *  )
 */
#define DEFINE_COMPONENT(X, attributes) \
    static void Start(); \
    static void Update(); \
    typedef struct tagComponent_##X { \
        Component base; \
        attributes \
    }Component_##X; \
    static inline Component* Component_New_##X() { \
        Component* c; \
        c = (Component*)malloc(sizeof(Component_##X)); \
        c->start = Start; \
        c->update = Update; \
        c->type = EID_##X; \
        return c; \
    }

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
    static inline Enity* Entity_New_##X(Entity* e) { \
        numChildren = 0; \
        numComponents = 0; \
        Entity_AddComponents(e, PP_NARG(__VA_ARGS__), __VA_ARGS__); \
    }

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

typedef void (*Component_StartFunc)();
typedef void (*Component_UpdateFunc)();

/**
 * The component structure. The collection of an entities' components defines
 * its behavior. This layout just defines the base structure of the component.
 * When you make your own components, ensure that the first fields match those
 * of this struct (even in order).
 */
typedef struct Component {
    Component_StartFunc start;
    Component_UpdateFunc update;
    unsigned type;
}Component;

/**
 * The entity structure. Entities represent all objects in the engine.
 */
typedef struct tagEntity {
    int numChildren;
    int numComponents;
    GSList* children;
    Component* components;
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

#ifdef __cplusplus
}
#endif
#endif

