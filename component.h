/******************************************************************************
 * component.h
 * This file defines the component type, which is the type that defines the 
 * behavior of pretty much everything in the engine.
 * This file should be included by components themselves.
 *
 * Bryce Wilson
 * Created June 24, 2013
 *****************************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H
#ifdef __cplusplus
extern "C" {
#endif


typedef struct Entity Entity;

typedef void (*Component_StartFunc)();
typedef void (*Component_UpdateFunc)();
typedef void (*Component_CollideFunc)(struct Entity* e);

/**
 * The component structure. The collection of an entities' components defines
 * its behavior. This layout just defines the base structure of the component.
 * When you make your own components, ensure that the first fields match those
 * of this struct (even in order).
 */
typedef struct Component {
    /* the start function, called upon adding this to an entity */
    Component_StartFunc start;
    /* the update function, called once a frame */
    Component_UpdateFunc update;
    /* the collide function, called when this component's entity collides with
     * another */
    Component_CollideFunc collide;
    /* the entity this componennt is attached to */
    struct Entity* entity;
    /* the ID of the component (tells what type it is) */
    unsigned id;
}Component;

/**
 * Retrieves a component that belongs to the same entity as self.
 * @param self the component requesting a sibling component.
 * @param id the component type (CID) to retrieve.
 * @return the component of the type requested or NULL if none exists.
 */
Component* Component_Get(Component* self, int id);

/* don't build these components, only insert their definitions */
#ifdef BUILD
#define BUILDING
#undef BUILD
#endif
/* define the macro for NOT building, and include all standard components */
#include "definecomponent.h"

#include "stdcomponents.h"

/* if we were building a component, continue */
#ifdef BUILDING
#undef BUILDING
#define BUILD
/* and redefine the macro so that building will work */
#include "definecomponent.h"
#endif

#ifdef __cplusplus
}
#endif
#endif

