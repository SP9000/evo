/******************************************************************************
 * collision.h
 * This file defines functions for checking collision between entities in the
 * engine.
 * 
 * Bryce Wilson
 * Created April 2013
 *****************************************************************************/
#ifndef _COLLISION_H
#define _COLLISION_H

#ifdef __cpluplus
extern "C" {
#endif

#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include "glib.h"
#include "stdcomponents.h"


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

/**
 * The currently colliding colliders in the game.
 */
extern GList* colliding;

#ifdef __cpluplus
}
#endif

#endif
