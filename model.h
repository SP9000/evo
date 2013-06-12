#ifndef _MODEL_H
#define _MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>

/* attribute types for the model */
#define ATTRIBUTE_NONE   0
#define ATTRIBUTE_VERTEX 1
#define ATTRIBUTE_COLOR  2
#define ATTRIBUTE_NORMAL 3
#define ATTRIBUTE_TEXCO  4

/* the # of floats each attribute uses */
#define ATTRIBUTE_VERTEX_SIZE 3
#define ATTRIBUTE_COLOR_SIZE  4
#define ATTRIBUTE_NORMAL_SIZE 3
#define ATTRIBUTE_TEXCO_SIZE  2

typedef float Vertex[3];
typedef float Normal[3];
typedef float Color[4];
typedef float Veretx2[2];

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
    /* buffers for each attribute of the model */
    float** attributes;
    /* a table of ATTRIBUTE_* ID's to tell the contents of attributes */
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
 * @return a pointer to the newly created model.
 */
Model* ModelNew(int numVertices);

/**
 * Free all the resources used by the given model.
 * @param m the model to free the resources of.
 */
void ModelFree(Model* m);

/**
  * Load a .ply model from file.
  * @param m the model to load into.
  * @param file the file to load the model from.
  */
void ModelLoadPLY(Model* m, char *file);

/**
 * Add a vertex to the given model. 
 * @param m the model to add the coordinate to.
 * @param v the vertex to append to the model
 */
void ModelAddVertex(Model* m, Vertex v);

/**
 * Add a color to the given model.
 * Be sure to call this and other AddAttribute commands before AddVertex since
 * AddVertex will increase the numVertices variable used to determine where to
 * add the other attributes 
 * @param m the model to add the color to.
 * @param c the color to append to the model.
 */
void ModelAddColor(Model* m, Color c);

/**
 * Add a triangle to the given model in the 0 z-plane.
 */
void ModelAddTriangle2(Model* m, float x1,float y1, float x2,float y2, float x3,float y3);

/**
 * Get the number of floats the attribute of the given ID uses.
 * @param id the ID of the attribute to get the number of floats used by.
 * @return the number of floats the given attribute uses or negative if unknown
 *  type.
 */
int ModelGetAttributeSize(int id);

/**
 * Set the specified attribute of this model.
 * @param dst the buffer of the attribute to set.
 * @param offset the number of the attribute to set within the buffer.
 * @param src the buffer to set the dst to.
 * @param type the type of the attribute to set.
 */
void ModelSetAttribute(float* dst, int dstOffset, float* src, int srcOffset, int type);

/**
 * Get the address of the specified attribute's buffer in the given model.
 * @param m the mode to find the buffer in.
 * @param attribute the attribute who's buffer is to be located.
 * @return the location of the buffer for the given attribute or NULL if the
 *  attribute doesn't exist in the given model.
 */
float* ModelGetAttributeBuffer(Model* m, int attribute);

#ifdef __cplusplus
}
#endif

#endif
