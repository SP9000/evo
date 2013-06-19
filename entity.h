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

#include "util.h"

#define ATTRIBUTE(X) X;

#define COMPONENT(X, attributes) \
    static void Start(); \
    static void Update(); \
    typedef struct tagComponent_##X { \
        Component base; \
        attributes \
    }Component_##X; \
    Component* Component_New_##X() { \
        Component* c; \
        c = (Component*)malloc(sizeof(Component_##X)); \
        c->start = Start; \
        c->update = Update; \
        return c; \
    }

typedef void (*Component_StartFunc)();
typedef void (*Component_UpdateFunc)();
typedef void (*Entity_StartFunc)();
typedef void (*Entity_UpdateFunc)();

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
    Entity_StartFunc start;
    Entity_UpdateFunc update;
    struct tagEntity* children;
    Component* components;
}Entity;

void Entity_New(int size, ...);
void Entity_AddComponent(Entity* e, Component* c);

#ifdef __cplusplus
}
#endif

#endif
