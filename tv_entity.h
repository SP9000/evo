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

#include "tv_types.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include "tv_draw.h"
#include "tv_scene.h"
#include "tv_util.h"
#include "cJSON.h"
#include "tv_component.h"
#include "transform.h"
#include "tv_message.h"


/** 
 * Create an entity with the given name and set its transform.
 * To create the entity call the function <name>().  Subsequent calls to 
 * <name>() will return a reference to this entity.
 * @param name the name of the entity.
 * @param px the x-position of the entity.
 * @param py the y-position of the entity.
 * @param pz the z-position of the entity.
 * @param sx the x-scale of the entity.
 * @param sy the y-scale of the entity.
 * @param sz the z-scale of the entity.
 * @param rx the x-rotation of the entity.
 * @param ry the y-rotation of the entity.
 * @param rz the z-rotation of the entity.
 */
#define ENTITY(entity_name, px, py, pz, sx, sy, sz, rx, ry, rz) \
	tv_entity* entity_name () { \
		static tv_entity* e = NULL; \
		{ \
		 if(e != NULL) { \
			return e; \
		 } \
		 e = tv_entity_new(NULL); \
		 e->transform.pos.x = px ; e->transform.pos.y = py ; e->transform.pos.z = pz ; \
		 e->transform.scale.x = sx ; e->transform.scale.y = sy ; e->transform.scale.z = sz ; \
		 e->transform.rot.x = rx ; e->transform.rot.y = ry ; e->transform.rot.z = rz ; e->transform.rot.w = 0.0f; \
		 e->name = #entity_name; \
		} \
	{
/** 
 * Create a prefab with the given name.
 * To create the prefab call the function <name>().  A new entity is created
 * with each call to <name>().
 * @param name the name of the prefab (the name of the function to call to
 *   produce an entity)
 */
#define PREFAB(prefab_name) \
	tv_entity* prefab_name () { \
		tv_entity* e = NULL; \
		{ \
		 e = tv_entity_new(NULL); \
		 e->transform.pos.x = 0 ; e->transform.pos.y = 0 ; e->transform.pos.z = 0 ; \
		 e->transform.scale.x = 1 ; e->transform.scale.y = 1 ; e->transform.scale.z = 1 ; \
		 e->transform.rot.x = 0 ; e->transform.rot.y = 0 ; e->transform.rot.z = 0 ; e->transform.rot.w = 0.0f; \
		 e->name = #prefab_name; \
		} \
	{
/**
 * Add a component of the given name to the current entity.
 * e.g:
 *   ENTITY(name, ...)
 *   ADDCOMPONENT(modelrenderer, r)
 * @param type the type of the component to add (e.g. TV_model)
 * @param name the name to refer to the component as for the rest of the entity
 *   definition.
 */
#define ADDCOMPONENT(type, name) \
	type * name = (type*)tv_entity_add_component(e, (tv_component*)type ## _new()); 

/**
 * End the current entity declartion.
 */
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

	/* tags are 1 bit each, so tag names must be 0x01, 0x02, 0x04, 0x08, ... */
	tvuint tags;

    tvuint numChildren;
    tvuint numComponents;
    struct tv_entity  *parent;
    tv_array /*tv_entity**/ *children;
	tv_array /*tv_component*/ *components;

	/* the name of the entity */
	tvchar* name;
}tv_entity;

/**
 * Initializes the entity system.
 * @return on success, 0, else an error code.
 */
int tv_entity_init();

/**
 * Delete all entities that have been created as well as their associated 
 * components.
 */
void tv_entity_quit();

/**
 * Create an empty entity.
 * @return an empty entity.
 */
tv_entity* tv_entity_new(tv_transform *transform);

/**
 * Destroy the given entity and all its children.
 * @param e the entity to destroy (in addition to all its components and 
 *  all its children).
 */
void tv_entity_destroy(tv_entity* e);

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
 * This is called once all the components have been added to an entity.
 * It calls the start function of every component.
 * @param e the entity to start.
 */
void tv_entity_start(tv_entity* e);

/**
 * Update all entities that have been created.
 */
void tv_entity_update_all();

/**
 * Get all active entities with the specified tag(s).
 */
tv_array* tv_entity_get_all_with_tag(tvuint tag);

/**
 * Instantiate a new entity identical to the given one.
 * @param e the entity to copy/create.
 */
void tv_entity_instantiate(tv_entity *e);

/**
 * Instantiate a new entity identical to the given one at the given position.
 * @param e the entity to copy/create.
 * @param at the location to instantiate the entity at.
 */
void tv_entity_instantiate_at(tv_entity *e, tv_vector3 at);


/**
 * A macro to retrieve and cast the desired component from an entity.
 */
#define tv_entity_get(X, Y) ((Component_##Y *)Entity_GetComponent(X, CID_##Y))

#define tv_entity_add_tag(entity, tag) (( entity )->tags |= ( tag ));
#define tv_entity_remove_tag(entity, tag) (( entity )->tags &= ~( tag ));
#define tv_entity_set_tags(entity, tags) (( entity )->tags = ( tags ));

#ifdef __cplusplus
}
#endif
#endif

