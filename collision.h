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
#include "glib.h"
#include "Gen/all.h"

/************************** types of colliders *******************************/
/* Box collider - the AABB represents the collider's dimensions. */
#define COLLIDER_BOX 1
/* Sphere collider - All dimensions of the AABB represent the radius. */
#define COLLIDER_SPHERE 2
/* Plane collider - the AABB's w and h dimensions represent the plane's */
#define COLLIDER_PLANE 3

typedef struct tagCollision {
    Component_Collider* col1;
    Component_Collider* col2;
}Collision;

/**
 * Initialize collision for use in the engine 
 */
void Collision_Init();

/**
 * Adds a collider to the internal list of things to be checked for collision
 * between each other.
 * @param col the collider to begin checking collision for.
 */
void Collision_AddCollider(Component_Collider* col);

/**
 * Detect collision between all the colliders that have been added to the game.
 */
void Collision_Detect();


#ifdef __cpluplus
}
#endif

#endif
