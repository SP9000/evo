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
#include "tv_types.h"
#include "tv_entity.h"
#include "tv_physics.h"
#include "tv_collider.h"


/************************** types of colliders *******************************/
/* Box collider - the AABB represents the collider's dimensions. */
#define COLLIDER_BOX 1
/* Sphere collider - All dimensions of the AABB represent the radius. */
#define COLLIDER_SPHERE 2
/* Plane collider - the AABB's w and h dimensions represent the plane's */
#define COLLIDER_PLANE 3

typedef struct {
	/* the colliders involved in the collision */
    tv_entity* col1;
    tv_entity* col2;
	/* the (or one) location that the collision occurred at. */
	tv_vector3 location;
}tv_collision;

typedef tvbool (*TvCollisionDetectFunc)(tv_entity*);
typedef void (*TvCollisionCollideFunc)(tv_entity*);

/**
 * Initialize collision for use in the engine 
 */
tvint tv_collision_init();


/**
 * Register the given collider with the collision detection handlers.
 * @param collider the collider to register for collision detection.
 */
void tv_collision_register_collider(TV_collider* collider);

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
void tv_collision_add_entity(tv_entity* e);

/**
 * Removes the specified entity from the collision detection routines.
 * @param e the entity to remove.
 */
void tv_collision_remove_entity(tv_entity* e);

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
void tv_entity_collide(tv_entity* e, tv_entity* other);

/**
 * Check collision with all colliders that've been added and the given collider.
 * @param c the collider to test collision with.
 * @return an array of all the collisions that occured.
 *    collider.
 */
tv_array* tv_collision_check(TV_collider* c);


/**
 * Check for collision between the two colliders.
 * @param c1 the first collider.
 * @param c2 the second collider.
 * @return TRUE if the colliders are overlapping, FALSE if not.
 */
tvbool tv_collider_check_collision(TV_collider *c1, TV_collider *c2, tv_collision* c);

#ifdef __cpluplus
}
#endif
#endif
