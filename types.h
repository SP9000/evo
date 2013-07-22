/*****************************************************************************/
/* types.h                                                                   */
/* This file contains definitions for various types that may be useful to a  */
/* variety of parts of the engine                                            */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June 25, 2013                                                    */
/*****************************************************************************/
#ifndef TYPES_H
#define TYPES_H
#ifdef __cplusplus
extern "C"
#endif

/* GLib has lots of useful types... */
#include <GL/glew.h>
#include <stdbool.h>
#include "glib.h"
#include "matrix.h"

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

typedef struct tagTexture {
    /* the ID of the texture itself (as given by glGenTextures) */
    GLuint id;
    /* the location of the texture in the shader program it's attacked to */
    GLuint loc;
    /* the handle of the sampler used this texture (given by glGenSamplers). */
    GLuint sampler;
}Texture;

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
