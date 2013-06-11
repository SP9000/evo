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
    Vertex* vertices;
    Normal* normals;
    Color* colors;

    /* The type of primitive to render as. e.g. GL_LINES, GL_TRIANGLES... */
    GLuint primitive;

    /* ID's for rendering with OpenGL-2.0+. */
    GLuint vertexVBOID, colorVBOID, normalVBOID;
    GLuint vao;

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


#ifdef __cplusplus
}
#endif

#endif
