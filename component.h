/*****************************************************************************/
/* component.h                                                               */
/* This file defines the component type, which is the type that defines the  */
/* behavior of pretty much everything in the engine.                         */
/* This file should be included by components themselves.                    */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created June 24, 2013                                                     */
/*****************************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "input.h"
#include "timer.h"
#include "types.h"
#include "util.h"

typedef struct Entity Entity;

#include "Gen/all.h"

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
    /* the awake function, called when the component is created */
    void (*Awake)();
    /* the start function, called upon adding this to an entity */
    void (*Start)();
    /* the update function, called once a frame */
    void (*Update)();
    /* the collide function, called when this component's entity collides with
     * another */
    void (*Collide)();
    /* the entity this componennt is attached to */
    struct Entity* entity;
    /* the ID of the component (tells what type it is) */
    unsigned id;
}Component;

/* gross forward declaration */
Component* Entity_GetComponent(Entity* e, int cid);

/**
 * Retrieves a component that belongs to the same entity as self.
 * @param self the component requesting a sibling component.
 * @param id the component type (CID) to retrieve.
 * @return the component of the type requested or NULL if none exists.
 */
Component* Component_Get(Component* self, int id);

/**
 * A convienience macro to retrieve the component of the specified type and 
 * cast it to that same type.
 */
#define Component_GetAs(TYPE) \
    (Component_##TYPE *)Component_Get((Component*)self, CID_##TYPE)

#ifdef __cplusplus
}
#endif
#endif

