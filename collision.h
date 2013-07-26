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
    TvComponent* col1;
    TvComponent* col2;
} TvCollision;

/**
 * Initialize collision for use in the engine 
 */
void tv_collision_init();

/**
 * Associates a given ID as a collider thereby adding components with that
 * id to the collision detection routines.
 * @param id the ID to register as a collider.
 */
void tv_collision_register_collider(void (*on_collision)(TvEntity*), tvuint cid);

/**
 * Adds a collider to the internal list of things to be checked for collision
 * between each other.
 * @param col the collider to begin checking collision for.
 */
void tv_collision_add_collider(TvComponent* col);

/**
 * Detect collision between all the colliders that have been added to the game.
 */
void tv_collision_detect();

/**
 * Call this every frame to update internal collision information as necessary.
 */
void tv_collision_update();

#ifdef __cpluplus
}
#endif
#endif
