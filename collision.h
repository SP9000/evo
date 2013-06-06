#ifndef _COLLISION_H
#define _COLLISION_H

#ifdef __cpluplus
extern "C" {
#endif

#include <malloc.h>
#include <stdbool.h>
#include "glib.h"

typedef struct tagNode {
    float x, y, z;
}Node;

/**
 * Axis-aligned bounding box.  This is what is used for checking collision
 * between objects.
 */
typedef struct tagAABB {
    float w, h, d;
}AABB;

/**
 * A struct containing relevant information for handing collision detection.
 */
typedef struct tagCollider {
    Node* pos;
    AABB* aabb;
    GList* colliding;
}Collider;

typedef struct tagCollision {
    Collider* col1;
    Collider* col2;
}Collision;

/**
 * Initialize collision for use in the engine 
 */
void CollisionInit();

/**
 * Adds a collider to the internal list of things to be checked for collision
 * between each other.
 * @param col the collider to begin checking collision for.
 */
void CollisionAddCollider(Collider* col);

/**
 * Detect collision between all the colliders that have been added to the game.
 */
void CollisionDetect();

/**
 * The currently colliding colliders in the game.
 */
extern GList* colliding;

#ifdef __cpluplus
}
#endif

#endif
