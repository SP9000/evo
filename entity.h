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
#include "transform.h"
#include "tv_message.h"

#define ENTITY(name, px, py, pz, sx, sy, sz, rx, ry, rz) \
	tv_entity* name () { \
		static tv_entity* e = NULL; \
		{ \
		 if(e != NULL) { \
			return e; \
		 } \
		 e = tv_entity_new(NULL); \
		 e->transform.pos.x = px ; e->transform.pos.y = py ; e->transform.pos.z = pz ; \
		 e->transform.scale.x = sx ; e->transform.scale.y = sy ; e->transform.scale.z = sz ; \
		 e->transform.rot.x = rx ; e->transform.rot.y = ry ; e->transform.rot.z = rz ; \
		} \
	{

#define COMPONENTS {

#define ADDCOMPONENT(type, name) \
	type * name = (type*)tv_entity_add_component(e, (tv_component*)type ## _new()); 

#define ENTITY_END \
	} \
		tv_entity_start(e); \
		return e; \
	} 

/**
 * The entity structure. Entities represent all objects in the engine.
 */
typedef struct tv_entity {
	tv_transform transform;
	TvAABB aabb;

	/* the ID of the collider component (0 if none) attached to the entity */
	tvuint collide;

    tvuint numChildren;
    tvuint numComponents;
    struct tv_entity  *parent;
    tv_array /*tv_entity**/ *children;
	tv_array /*tv_component*/ *components;
}tv_entity;

/**
 * Initializes the entity system.
 * @return on success, 0, else an error code.
 */
int tv_entity_init();

/**
 * Create an empty entity.
 * @return an empty entity.
 */
tv_entity* tv_entity_new(tv_transform *transform);

/**
 * Add the given component to the given entity.
 * @param e the entity to add the component to.
 * @param c the component to add to the entity.
 * @param returns NULL if failed or there was already a component of the 
 *  given type, otherwise returns the address of the component.
 */
tv_component* tv_entity_add_component(tv_entity* e, tv_component* c);

/**
 * Add the given entity as a child to the given parent.
 * @param parent the entity to add the child to.
 * @param child the entity to add to the parent.
 */
void tv_entity_add_child(tv_entity* parent, tv_entity* child);

/**
 * Get the component of the specified type from the entity.
 * @param e the entity to retrieve the desired component from.
 * @param cid the ID of the type of the component to get.
 * @return the component if it exists in the entity, NULL otherwise.
 */
tv_component* tv_entity_get_component(tv_entity* e, tvuint cid);


/**
 * Sends a message from one entity to another.
 * This function sends a message to another entity notifying it of some event.
 * The message is then sent to each component of the receiving entity.
 * @param sender the entity that is sending the message.
 * @param receiver the entity that is to receive the message.
 * @param message_type the nature of the message being sent.
 * @param message the message data.
 */
void tv_entity_send_message(tv_entity *sender, tv_entity *receiver, tv_message_type message_type, tv_message message); 

/**
 * Receives a message that is sent by another entity.
 * This function is what is called when a message is sent from another entity.
 * It notifies the receiver's children and components that a message has 
 * arrived and leaves it to them to handle it.
 * @param sender the entity that is sending the message.
 * @param receiver the entity that is to receive the message.
 * @param message_type the nature of the message being sent.
 * @param message the message data.
 */
void tv_entity_send_message(tv_entity *sender, tv_entity *receiver, tv_message_type message_type, tv_message message);

/**
 * This is called once all the components have been added to an entity.
 * It calls the start function of every component.
 * @param e the entity to start.
 */
void tv_entity_start(tv_entity* e);

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

