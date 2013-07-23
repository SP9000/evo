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
#include <stdbool.h>
#include "glib.h"
#include "matrix.h"
#include "texture.h"

/* the # of floats each attribute uses */
#define MODEL_ATTRIBUTE_VERTEX_SIZE 3
#define MODEL_ATTRIBUTE_COLOR_SIZE  4
#define MODEL_ATTRIBUTE_NORMAL_SIZE 3
#define MODEL_ATTRIBUTE_TEXCO_SIZE  2

/* the number of render layers */
#define RENDER_LAYER_COUNT  2
/* the different layers for rendering */
#define RENDER_LAYER_MAIN   0
#define RENDER_LAYER_GUI    1

/* attribute types for the model */
enum {
    MODEL_ATTRIBUTE_NONE,
    MODEL_ATTRIBUTE_VERTEX,
    MODEL_ATTRIBUTE_COLOR,
    MODEL_ATTRIBUTE_NORMAL,
    MODEL_ATTRIBUTE_TEXCO   
};
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

/**
 * A type for representing a rectangle.
 */
typedef struct tagRect {
    float x, y;
    float w, h;
}Rect;

/**
 * A type for holding data useful for basic GUI rendering 
 */
typedef struct tagGUIContent {
    GString* text;
    Texture texture;
}GUIContent;

/**
 * Checks if the given rectangle contains the given point.
 * @param r the rectangle to check for the point within.
 * @param point the point to determine if is in the bounds of the rect or not.
 * @return nonzero if the rectangle does contain the point, else zero.
 */
int Rect_ContainsPoint(Rect* r, Vector2* point);
/**
 * Checks if the given rectangle overlaps the other given rectangle.
 * @param r1 the first rectangle.
 * @param r2 the...second...rectangle.
 * @return nonzero if the rectangles overlap, else zero.
 */
int Rect_Overlaps(Rect* r, Rect* r2);

#ifdef __cplusplus
}
#endif
#endif
