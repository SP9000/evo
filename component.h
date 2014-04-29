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
#include "tv_alloc.h"
#include "tv_message.h"

typedef struct tv_entity tv_entity;
typedef struct tv_component tv_component;

/**
 * The maximum number of handlers that may be registered in the engine. This
 * ought to be PLENTY.
 */
#define TV_COMPONENT_MAX_HANDLERS 128

/**
 * The maximum number of pre-handlers (obviously this must be smaller than
 * the maximum number of total handlers).
 */
#define TV_COMPONENT_MAX_PRE_HANDLERS (TV_COMPONENT_MAX_HANDLERS / 2)

/**
 * The maximum number of post handlers.
 */
#define TV_COMPONENT_MAX_POST_HANDLERS (TV_COMPONENT_MAX_HANDLERS - TV_COMPONENT_MAX_PRE_HANDLERS)

/* the number of stages there are before update */
#define TV_COMPONENT_MAX_PRE_STAGES 8
/* the number of stages there are after update */
#define TV_COMPONENT_MAX_POST_STAGES 8
/* the total number of update stages there are (excluding update) */
#define TV_COMPONENT_MAX_STAGES (TV_COMPONENT_MAX_PRE_STAGES + TV_COMPONENT_MAX_POST_STAGES)

#define TV_UPDATE_STAGE_MAIN_RENDER 1
#define TV_UPDATE_STAGE_GUI_RENDER 2
/*****************************************************************************/
/*Component macros                                                           */
#define COMPONENT(component_prefix, parent_prefix) \
	tvuint component_prefix ## _id(); \
	void component_prefix ## _register(); \
	typedef struct { \
		parent_prefix base;
#define ENDCOMPONENT(component_prefix) \
	} component_prefix; \
	component_prefix * component_prefix ## _new(); \
	void component_prefix ## _init(component_prefix *component);

#define COMPONENT_START(component_prefix) \
	static void start(tv_component *self_component) { \
		component_prefix *self = (component_prefix*)self_component;
#define END_COMPONENT_START \
	}
#define COMPONENT_UPDATE(component_prefix) \
	static void update(tv_component *self_component) { \
		component_prefix *self = (component_prefix*)self_component;
#define END_COMPONENT_UPDATE \
	}

#define COMPONENT_NEW(component_prefix, parent_prefix) \
	static void start(tv_component *self); \
	static void update(tv_component *self); \
	static tvuint tv_cid_ ## component_prefix = 0; \
	tvuint component_prefix ## _id() { \
		return tv_cid_ ## component_prefix; \
	} \
	void component_prefix ## _register() { \
		if(!(parent_prefix ## _id())) { \
			parent_prefix ## _register(); \
		} \
		if(!tv_cid_ ## component_prefix) { \
			tv_component_register_id(&tv_cid_ ## component_prefix, parent_prefix ## _id()); \
		} \
	} \
	component_prefix * component_prefix ## _new() { \
		component_prefix *self = (component_prefix *)tv_alloc(sizeof(component_prefix)); \
		component_prefix ## _init(self); \
		return self; \
	} \
	void component_prefix ## _init(component_prefix * self) {
		
#define END_COMPONENT_NEW(component_prefix) \
		((tv_component*)self)->entity = NULL; \
		((tv_component*)self)->id = tv_cid_ ## component_prefix; \
		((tv_component*)self)->Start = start; \
		((tv_component*)self)->Update = update; \
	}

/*****************************************************************************/
/*Handler macros 														     */
#define HANDLER(handler_name, parent_name) \
	tvuint handler_name ## _id(); \
	void handler_name ## _register(); \
	typedef struct { \
		parent_name base;
#define ENDHANDLER(handler_name) \
	} handler_name; \
	handler_name * handler_name ## _new(); 

#define HANDLER_START(handler_name) \
	static void start(tv_component *self_component) { \
		handler_name *self = (handler_name*)self_component;
#define END_HANDLER_START \
	}

#define HANDLER_UPDATE(handler_name) \
	static void update(tv_component *self_component) { \
		handler_name *self = (handler_name*)self_component;
#define END_HANDLER_UPDATE \
	}

#define HANDLER_NEW(handler_name, parent_name, handler_func, pre_update) \
	static void start(tv_component *self); \
	static void update(tv_component *self); \
	static tvuint tv_cid_ ## handler_name = 0; \
	static void handler_func(tv_component *self); \
	tvuint handler_name ## _id() { \
		return tv_cid_ ## handler_name; \
	} \
	void handler_name ## _register() { \
		if(!(parent_name ## _id())) { \
			parent_name ## _register(); \
		} \
		if(!tv_cid_ ## handler_name) { \
			tv_component_register_handler(&tv_cid_ ## handler_name, parent_name ## _id(), handler_func, pre_update); \
		} \
	} \
	handler_name * handler_name ## _new() { \
		handler_name *self = (handler_name *)tv_alloc(sizeof(handler_name)); 
#define END_HANDLER_NEW(handler_name) \
		((tv_component*)self)->entity = NULL; \
		((tv_component*)self)->id = tv_cid_ ## handler_name; \
		tv_component_register_to_handler(((tv_component*)self)->id, (tv_component*)self, 1); \
		((tv_component*)self)->Start = start; \
		((tv_component*)self)->Update = update; \
		return self; \
	}

#define TVCALL(METHOD, ...) METHOD (self, __VA_ARGS__ )

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

	TvTransform *transform;
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
 * Get a unique ID to use to refer to a type of component.
 * @param id upon return contains a unique ID.
 * @param parent_id the ID of the component that the ID to register inherits 
 *   from, or 0 if this is a top-level component (inherits only tv_component)
 */
void tv_component_register_id(tvuint *id, tvuint parent_id);

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
 * @param pre_update if TRUE the function is called prior to the component's 
 *	update function, if FALSE, it is called afterward.
 */
void tv_component_register_handler(tvuint *id, tvuint parent_id, void (*func)(tv_component*), tvbool pre_update);

/**
 * Registers the given component to the handler given by the given associated ID.
 * @param handler_id the ID of the handler to execute upon this component
 * @param component the component to add to the given handler.
 * @param pre_update if TRUE, this registers to the corresponding pre-update
 *   if FALSE, post.
 */
void tv_component_register_to_handler(tvuint handler_id, tv_component *component, tvbool pre_update);

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
 * Receives a message sent by an outside entity.
 * When another entity sends a message, the receiving entity is notified and 
 * this function is called on all components of that entity.
 * @param sender the entity that sent the message.
 * @param message_type the nature of the message that is being received.
 * @param message the message data.
 */
void tv_component_receive_message(tv_entity *sender, tv_message_type message_type, tv_message message);

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
		tv_component_register_id(&tv_cid_component, 0); 
	} 
} 



#ifdef __cplusplus
}
#endif
#endif

