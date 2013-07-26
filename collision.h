/*****************************************************************************/
/* collision.h                                                               */
/* This file defines functions for checking collision between entities in    */
/* the engine.                                                               */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created April 2013                                                        */
/*****************************************************************************/
#ifndef COLLISION_H
#define COLLISION_H

#ifdef __cpluplus
extern "C" {
#endif

#include <malloc.h>
#include <stdio.h>
#include "entity.h"
#include "types.h"


/************************** types of colliders *******************************/
/* Box collider - the AABB represents the collider's dimensions. */
#define COLLIDER_BOX 1
/* Sphere collider - All dimensions of the AABB represent the radius. */
#define COLLIDER_SPHERE 2
/* Plane collider - the AABB's w and h dimensions represent the plane's */
#define COLLIDER_PLANE 3

typedef struct tagTvCollision {
    TvEntity* col1;
    TvEntity* col2;
} TvCollision;


typedef tvbool (*TvCollisionDetectFunc)(TvEntity*);
typedef void (*TvCollisionCollideFunc)(TvEntity*);

/**
 * Initialize collision for use in the engine 
 */
void tv_collision_init();

/**
 * Associates a given ID as a collider thereby adding components with that
 * id to the collision detection routines.
 * @param detect_func the function to call once two colliders AABB's overlap to
 *	determine if a collision happened or not.
 * @param id the ID to register as a collider.
 */
void tv_collision_register_collider(TvCollisionDetectFunc detect_func, tvuint cid);

/**
 * Associates a given ID with a function that should be called when a 
 * collision occurs.
 * @param on_collision the function to call once a collision has occurred.
 * @param id the ID to register a collide function with.
 */
void tv_collision_register_component(TvCollisionCollideFunc on_collision, tvuint id);

/**
 * Adds the entity to the internal list of things to be checked for collision.
 * @param e the entity to check collision for from here on.
 */
void tv_collision_add_entity(TvEntity* e);

/**
 * Detect collision between all the colliders that have been added to the game.
 */
void tv_collision_detect();

/**
 * Call this every frame to update internal collision information as necessary.
 */
void tv_collision_update();

/**
 * Call the collide function on all components for the given entity.
 * @param e the entity to call the collide function for.
 * @param other the entity to pass as a parameter to the collide function.
 */
void tv_entity_collide(TvEntity* e, TvEntity* other);

#ifdef __cpluplus
}
#endif
#endif
