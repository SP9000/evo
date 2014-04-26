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
extern "C" {
#endif

/* picky stuff */
#define ssize_t size_t

#include <GL/glew.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tv_array.h"
#include "tv_hash.h"
#include "tv_list.h"
#include "tv_string.h"
#include "matrix.h"
#include "tv_vector.h"

/* macros */
#define TV_DATA_PTR(i) ((void*)(i))

/* boolean */
typedef int tvbool;
#define false 0
#define true 1

/* fixed size types */
typedef uint8_t tvbyte;
typedef uint16_t tvword;
typedef uint32_t tvdword;
typedef uint64_t tvqword;

/* primitive types */
typedef char tvchar;
typedef int tvint;
typedef float tvfloat;
typedef double tvdouble;
typedef unsigned tvuint;
typedef uint32_t tvu32;

/* data structures */
typedef UT_array TvArray;
typedef UT_hash_table TvHashTable;
typedef UT_hash_handle TvHashHandle;

typedef void* tvpointer;

typedef UT_string TvString;

/* the number of render layers */
#define RENDER_LAYER_COUNT  2
/* the different layers for rendering */
#define RENDER_LAYER_MAIN   0
#define RENDER_LAYER_GUI    1

/**
 * A type for representing the position, scale, and rotation of an object.
 */
typedef struct tagTvTransform {
	tv_vector3 position;
	tv_vector3 scale;
	tv_vector3 rotation;
}TvTransform;

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
 * Checks if the given rectangle contains the given point.
 * @param r the rectangle to check for the point within.
 * @param point the point to determine if is in the bounds of the rect or not.
 * @return nonzero if the rectangle does contain the point, else zero.
 */
int tv_rect_contains(TvRect* r, tv_vector2* point);
/**
 * Checks if the given rectangle overlaps the other given rectangle.
 * @param r1 the first rectangle.
 * @param r2 the...second...rectangle.
 * @return nonzero if the rectangles overlap, else zero.
 */
int tv_rect_overlaps(TvRect* r1, TvRect* r2);

extern UT_icd ut_float_icd;
extern UT_icd ut_short_icd;


#ifdef __cplusplus
}
#endif
#endif
