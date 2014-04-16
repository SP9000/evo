/*****************************************************************************/
/* Collider.c																 */
/* The collider component.                                                   */
/* This component contains information that allows the entity it is attached */
/* to be checked for collision with other entities that have colliders.      */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June, 2013                                                       */
/*****************************************************************************/
#ifndef COLLIDER_H
#define COLLIDER_H

#include "All.h"
typedef struct tagCollider {
	TvComponent base;
	TvCollisionCollideFunc Collide;
	TvCollisionDetectFunc Detect;
}Collider;

Collider* collider_new();

#endif