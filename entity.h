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
#include "util.h"

#define ATTRIBUTE(X) X;
#define COMPONENT(X, attributes) \
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
        return c; \
    }

#define ENTITY(X, ...) \
    static inline Enity* Entity_New_##X(Entity* e) { \
        numChildren = 0; \
        numComponents = 0; \
        Entity_AddComponents(e, PP_NARG(__VA_ARGS__), __VA_ARGS__); \
    }

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
}Component;

/**
 * The entity structure. Entities represent all objects in the engine.
 */
typedef struct tagEntity {
    int numChildren;
    int numComponents;
    struct tagEntity* children;
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

