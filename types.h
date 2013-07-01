/******************************************************************************
 * types.h
 * This file contains definitions for various types that may be useful to a 
 * variety of parts of the engine
 * 
 * Bryce Wilson
 * Created: June 25, 2013
 *****************************************************************************/
#ifndef TYPES_H
#define TYPES_H
#ifdef __cplusplus
extern "C"
#endif

/* GLib has lots of useful types... */
#include "glib.h"


/**
 * A basic type for representing a point in 2-dimensional space.
 */
typedef struct tagVector2 {
    float x, y;
}Vector2;

/**
 * A basic type for representing a point in 3-dimensional space.
 */
typedef struct tagVector3 {
    float x, y, z;
}Vector3;

/**
 * Axis-aligned bounding box.  This is what is used for checking collision
 * between objects.
 */
typedef struct tagAABB {
    float w, h, d;
}AABB;

typedef struct tagRect {
    float x, y;
    float w, h;
}Rect;

#ifdef __cplusplus
}
#endif
#endif
