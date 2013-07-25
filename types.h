/*****************************************************************************/
/* types.h                                                                   */
/* This file contains definitions for various types that may be useful to a  */
/* variety of parts of the engine.                                           */
/* A types.c file contains functions for performing various operations on    */
/* these types without polluting the codebase with a million other files.    */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June 25, 2013                                                    */
/*****************************************************************************/
#ifndef TYPES_H
#define TYPES_H
#ifdef __cplusplus
extern "C"
#endif

#include <GL/glew.h>
#include <glib/glib.h>
#include <string.h>
#include "matrix.h"
#include "texture.h"

/* boolean */
typedef int bool;
#define false 0
#define true 1

/* primitive types */
typedef char tvchar;
typedef int tvint;
typedef float tvfloat;
typedef unsigned tvuint;
typedef bool tvbool;
typedef uint_32 tvu32;

typedef GArray TvArray;
typedef GList TvList;
typedef GHashTable TvHashTable;
typedef GQueue TvQueue;

/* the number of render layers */
#define RENDER_LAYER_COUNT  2
/* the different layers for rendering */
#define RENDER_LAYER_MAIN   0
#define RENDER_LAYER_GUI    1

/**
 * A basic type for representing a point in 2-dimensional space.
 */
typedef struct tagVector2 {
    float x, y;
}TvVector2;

/**
 * A basic type for representing a point in 3-dimensional space.
 */
typedef struct tagVector3 {
    float x, y, z;
}TvVector3;

/**
 * Axis-aligned bounding box.  This is what is used for checking collision
 * between objects.
 */
typedef struct tagAABB {
    float w, h, d;
}TvAABB;

/**
 * A type for representing a rectangle.
 */
typedef struct tagRect {
    float x, y;
    float w, h;
}TvRect;

/**
 * A type for holding data useful for basic GUI rendering 
 */
typedef struct tagGUIContent {
    GString* text;
    Texture texture;
}TvGUIContent;

/**
 * Checks if the given rectangle contains the given point.
 * @param r the rectangle to check for the point within.
 * @param point the point to determine if is in the bounds of the rect or not.
 * @return nonzero if the rectangle does contain the point, else zero.
 */
int tv_rect_contains(TvRect* r, TvVector2* point);
/**
 * Checks if the given rectangle overlaps the other given rectangle.
 * @param r1 the first rectangle.
 * @param r2 the...second...rectangle.
 * @return nonzero if the rectangles overlap, else zero.
 */
int tv_rect_overlaps(TvRect* r1, TvRect* r2);

#ifdef __cplusplus
}
#endif
#endif
