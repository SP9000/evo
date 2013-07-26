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

typedef struct TvEntity TvEntity;
typedef struct TvComponent TvComponent;

/**
 * The component structure. The collection of an entities' components defines
 * its behavior. This layout just defines the base structure of the component.
 * When you make your own components, ensure that the first fields match those
 * of this struct (even in order).
 */
typedef struct TvComponent {
    /* the awake function, called when the component is created */
    void (*Awake)(TvComponent*);
    /* the start function, called upon adding this to an entity */
    void (*Start)(TvComponent*);
    /* the update function, called once a frame */
    void (*Update)(TvComponent*);
    /* the collide function, called when this component's entity collides with
     * another */
    void (*Collide)(TvComponent*, TvEntity*);
    /* the entity this componennt is attached to */
    struct TvEntity* entity;
    /* the ID of the component (tells what type it is) */
    unsigned id;
}TvComponent;

/* gross forward declaration */
TvComponent* tv_entity_get_component(TvEntity* e, tvuint id);

/**
 * Retrieves a component that belongs to the same entity as self.
 * @param self the component requesting a sibling component.
 * @param id the component type (CID) to retrieve.
 * @return the component of the type requested or NULL if none exists.
 */
TvComponent* tv_component_get(TvComponent* self, tvuint id);

/**
 * A convienience macro to retrieve the component of the specified type and 
 * cast it to that same type.
 */
#define tv_component_getas(TYPE) \
    (Component_##TYPE *)tv_component_getas((Component*)self, CID_##TYPE)

#ifdef __cplusplus
}
#endif
#endif

