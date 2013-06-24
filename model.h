/******************************************************************************
 * model.h
 * Contains definitions for the model and the types it uses as well as 
 * functions to create/load/manage models. Models are used for nearly all
 * objects rendered in the engine including text and other GUI functionaliy.
 *
 * Bryce Wilson
 * created: April, 2013
 *****************************************************************************/
#ifndef _MODEL_H
#define _MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "material.h"

/* attribute types for the model */
#define MODEL_ATTRIBUTE_NONE   0
#define MODEL_ATTRIBUTE_VERTEX 1
#define MODEL_ATTRIBUTE_COLOR  2
#define MODEL_ATTRIBUTE_NORMAL 3
#define MODEL_ATTRIBUTE_TEXCO  4

/* the # of floats each attribute uses */
#define MODEL_ATTRIBUTE_VERTEX_SIZE 3
#define MODEL_ATTRIBUTE_COLOR_SIZE  4
#define MODEL_ATTRIBUTE_NORMAL_SIZE 3
#define MODEL_ATTRIBUTE_TEXCO_SIZE  2

typedef float Vertex[MODEL_ATTRIBUTE_VERTEX_SIZE];
typedef float Normal[MODEL_ATTRIBUTE_NORMAL_SIZE];
typedef float Color[MODEL_ATTRIBUTE_COLOR_SIZE];
typedef float Texco[MODEL_ATTRIBUTE_TEXCO_SIZE];

/**
 * Model vertex structure.
 * This defines the structure that contains all the data represented by each
 * vertex in the model. It is mostly used for adding/modifying vertices in the
 * model either through Lua or C.
 */
typedef struct ModelVertex {
    Vertex position;
    Normal normal;
    Color color;
}ModelVertex;


/**
 * Model structure.
 * This is holds the data for the data for the position, normal, and color for
 * each vertex in the model.
 */
typedef struct tagModel {
    /* the material associated with this model. */
    Material mat;
    /* buffers for each attribute of the model */
    float** attributes;
    /* a table of MODEL_ATTRIBUTE_* ID's to tell the contents of attributes */
    int* attributeTable;
    /* the # of unique per-vertex attributes */
    int numAttributes;

    /* VBO ID's for rendering with OpenGL-2.0+. */
    GLuint* vboIDs;
    GLuint vao;

    /* The type of primitive to render as. e.g. GL_LINES, GL_TRIANGLES... */
    GLuint primitive;

    /* Current number of faces in the model. */
    unsigned int numFaces;
    
    /* The offset of the next unassigned vertex. */
    unsigned int numVertices;

    /* Submodels of this model. For example: a leg may be a subgroup of body */
    struct tagModel* subgroups;
}Model;

/**
 * Create a new model.
 * Creates and initializes a new empty model.
 */
Model* Model_New();

/**
 * Free all the resources used by the given model.
 * @param m the model to free the resources of.
 */
void Model_Free(Model* m);

/**
  * Load a .ply model from file.
  * @param m the model to load into.
  * @param file the file to load the model from.
  */
void Model_LoadPLY(Model* m, char *file);

/**
 * Add the buffer of the given attribute type to the model.
 * @param m the model to add the attribute to.
 * @param attribute the type of the attribute to add.
 * @param data the data to buffer (an array of floats ATTRIBUTE_XX_SIZE *
 *   numVertices) floats long.
 */
void Model_BufferAttribute(Model* m, int attribute,  float* data);

/**
 * Add an attribute of the the given type to the model.
 * This function allocates a buffer of numVertices size for the given attribute.
 * to determine where to place the other attributes.
 * @param m the model to add the attribute to.
 * @param val the value to append to the given attribute buffer.
 */
void Model_AddAttribute(Model* m, int attribute);

/**
 * Set the specified attribute of the given model to the specified value.
 * @param m the model to set the attribute of.
 * @param attribute the type of the attribute to set.
 * @param offset the vertex-offset of the attribute to set.
 * @param val the value to set the attribute to.
 */
void Model_SetAttribute(Model* m, int attribute, int offset, float* val);

/**
 * Get the number of floats the attribute of the given ID uses.
 * @param id the ID of the attribute to get the number of floats used by.
 * @return the number of floats the given attribute uses or negative if unknown
 *  type.
 */
int Model_GetAttributeSize(int id);

/**
 * Set the specified attribute of this model.
 * @param dst the buffer of the attribute to set.
 * @param offset the number of the attribute to set within the buffer.
 * @param src the buffer to set the dst to.
 * @param type the type of the attribute to set.
 * TODO: kinda janky name. probably not a function to be used outside model.c.
 */
void Model_CopyAttribute(float* dst, int dstOffset, float* src, int srcOffset, int type);

/**
 * Get the address of the specified attribute's buffer in the given model.
 * @param m the mode to find the buffer in.
 * @param attribute the attribute who's buffer is to be located.
 * @return the location of the buffer for the given attribute or NULL if the
 *  attribute doesn't exist in the given model.
 */
float* Model_GetAttributeBuffer(Model* m, int attribute);

/**
 * Set the material of the given model to the given material.
 * @param m the model to set the material of.
 * @param mat the material to set the model's material property to.
 */
void Model_SetMaterial(Model* m, Material* mat);

#ifdef __cplusplus
}
#endif

#endif
