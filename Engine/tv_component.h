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

#include "tv_macros.h"
#include "tv_input.h"
#include "tv_time.h"
#include "tv_types.h"
#include "tv_util.h"
#include "tv_alloc.h"
#include "tv_message.h"
#include "tv_vector.h"
#include "transform.h"

typedef struct tv_entity tv_entity;
typedef struct tv_component tv_component;

/**
 * The maximum number of handlers that may be registered in the engine. This
 * ought to be PLENTY.
 */
#define TV_COMPONENT_MAX_HANDLERS 128

#define TV_COMPONENT_TRANSFORM_ID TV_COMPONENT_MAX_HANDLERS

/**
 * The maximum number of pre-handlers (obviously this must be smaller than
 * the maximum number of total handlers).
 */
#define TV_COMPONENT_MAX_PRE_HANDLERS (TV_COMPONENT_MAX_HANDLERS / 2)
/**
 * The maximum number of post handlers.
 */
#define TV_COMPONENT_MAX_POST_HANDLERS (TV_COMPONENT_MAX_HANDLERS - TV_COMPONENT_MAX_PRE_HANDLERS)
/*
 * The maximum number of component types that may be registered.
 */
#define TV_COMPONENT_MAX_COMPONENTS 100000

/* the number of stages there are before update */
#define TV_COMPONENT_MAX_PRE_STAGES 8
/* the number of stages there are after update */
#define TV_COMPONENT_MAX_POST_STAGES 8
/* the total number of update stages there are (excluding update) */
#define TV_COMPONENT_MAX_STAGES (TV_COMPONENT_MAX_PRE_STAGES + TV_COMPONENT_MAX_POST_STAGES)

#define TV_UPDATE_STAGE_MAIN_RENDER 1
#define TV_UPDATE_STAGE_GUI_RENDER 2

/**
 * The component structure. The collection of an entities' components defines
 * its behavior. This layout just defines the base structure of the component.
 * When you make your own components, ensure that the first fields match those
 * of this struct (even in order).
 */
typedef struct tv_component {
    /* the awake function, called when the component is created */
    void (*Awake)(tv_component*);
    /* the start function, called upon adding this to an entity */
    void (*Start)(tv_component*);
    /* the update function, called once a frame */
    void (*Update)(tv_component*);
	/* the destroy function, called when component is deleted */
	void (*Destroy)(tv_component*);
	/* the message handler, called when this component's entity receives a 
	 * message */
	void (*MessageHandler)(tv_message_type, tv_message);
	/* the collide function, called when this component's entity collides with
     * another */
	void (*Collide)(tv_component*, tv_entity*);

    /* the ID of the component (tells what type it is) */
    tvuint id;
    /* the entity this componennt is attached to */
    struct tv_entity* entity;
	/* the ID of the component this component inherits from. */
	tvuint parent_ids;

	tv_transform *transform;
}tv_component;

/**
 * The component handler function. Registered handlers call a function on 
 * all the components of the registered type each frame.
 */
typedef void (*tv_component_handler)(tv_component* c);

/**
 * An ICD for storing the component handlers in TvArrays.
 */
extern UT_icd tv_component_handler_icd;

/**
 * Initialize the component system.
 * @return 0 upon success, else non-zero.
 */
int tv_component_init();

/**
 * Copy the given component and return the copy.
 * @param c the component to copy. 
 * @return a copy of the component given.
 */
tv_component *tv_component_copy(tv_component* c);

/**
 * Notify the engine that a component was added.
 * @param c the component that was added.
 */
void tv_component_notify_add(tv_component* c);

/**
 * You should call this after freeing all a component's resources.
 * This tells the engine that the component was removed.
 * @param c the component to free.
 */
void tv_component_free(tv_component *c);

/**
 * Get a unique ID to use to refer to a type of component.
 * @param id upon return contains a unique ID.
 * @param parent_id the ID of the component that the ID to register inherits 
 *   from, or 0 if this is a top-level component (inherits only tv_component)
 * @param size the size (in bytes) of the component to register.
 */
void tv_component_register_id(tvuint *id, tvuint parent_id, tvuint size);

/**
 * Register a component id to call a designated function every frame.
 * This function assigns an ID for the component type as well, so for
 * components that need to be registered do NOT call tv_component_register_id.
 * Component types that are registered are assigned the lowest value ID's
 * since they are used as indices for the internal array of handlers.
 * @param id the component ID to register the function for as well as 
 *  identify the component.
 * @param parent_id the ID of the component that the ID to register inherits 
 *   from, or 0 if this is a top-level component (inherits only tv_component)
 * @param func the function to call on the registered component type.
 * @param stage The stage that this handler performs its update.
 * @param size the size (in bytes) of the handler.
 */
void tv_component_register_handler(tvuint *id, tvuint parent_id, void (*func)(tv_component*), tvuint stage, tvuint size);

/**
 * Registers the given component to the handler given by the given associated ID.
 * @param handler_id the ID of the handler to execute upon this component
 * @param component the component to add to the given handler.
 */
void tv_component_register_to_handler(tvuint handler_id, tv_component *component);

/**
 * Run all component handlers that execute prior to the Update method.
 * The order that the handlers will execute is given by the order they were registered.
 */
void tv_component_update_pre_handlers();

/**
 * Run all component handlers that execute after to the Update method.
 * The order that the handlers will execute is given by the order they were registered.
 */
void tv_component_update_post_handlers();

/* gross forward declaration */
tv_component* tv_entity_get_component(tv_entity* e, tvuint id);

/**
 * Checks if the given component inherits the component associated with the given 
 * id.
 * @param component the component to check.
 * @param id the ID of the component to check inheritance with.
 * @return if the component does inherit the associated component ID, returns 
 *  TRUE, otherwise returns FALSE.
 */
tvbool tv_component_inherits(tv_component* component, tvuint id);

/**
 * Retrieves a component that belongs to the same entity as self.
 * @param self the component requesting a sibling component.
 * @param id the component type (CID) to retrieve.
 * @return the component of the type requested or NULL if none exists.
 */
tv_component* tv_component_get(tv_component* self, tvuint id);

/**
 * Get all the components of a given type that have been registered.
 * @param id the ID of the type to get the components of.
 * @return an array of the components of the given type.
 */
tv_array *tv_component_get_all_of_type(tvuint id);

/**
 * A debug function that ensures a component of the specified ID is attached
 * to the same entity as the given component.
 * @param component the component who's entity must contain a component of the
 *  given type.
 * @param id the ID of the component that must belong to the given component's
 *  entity.
 */
static void tv_component_require(tv_component *component, tvuint id) {assert(tv_component_get(component, id) != NULL);}

/*****************************************************************************/
/**
 * A dummy function to register the transform "component".
 */
static void tv_transform_register() {}

/**
 * The transform component (being a fake component in the first place, gets
 * special treatment).
 * This returns the ID of the transform component.
 * @return the ID that the transform "component" is registered to.
 */
static tvuint tv_transform_id() {return TV_COMPONENT_TRANSFORM_ID;}

/**
 * A convienience macro to retrieve the component of the specified type and 
 * cast it to that same type.
 */
#define tv_component_getas(TYPE) \
    (Component_##TYPE *)tv_component_getas((Component*)self, CID_##TYPE)

extern tvuint tv_cid_component;
static tvuint tv_component_id() { 
	return tv_cid_component;
} 
static void tv_component_register() { 
	if(!tv_cid_component) {
		tv_component_register_id(&tv_cid_component, 0, sizeof(tv_component)); 
	} 
} 

#ifdef __cplusplus
}
#endif
#endif

