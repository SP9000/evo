/******************************************************************************
 * material.h
 * Contains the definition of the material type and functions to load/manage
 * materials. Platform dependent code (OpenGL) is used in this system.
 * Bryce Wilson
 * created: June 18, 2013
 *****************************************************************************/
#ifndef MATERIAL_H
#define MATERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <GL/glew.h>
#include "glib.h"
#include "cJSON.h"
#include "util.h"

typedef struct __attribute__((__packed__)) tagTexel {
    GLbyte r;
    GLbyte g;
    GLbyte b;
    GLbyte a;
}Texel;

typedef struct tagTexture {
    /* the ID of the texture itself (as given by glGenTextures) */
    GLuint id;
    /* the location of the texture in the shader program it's attacked to */
    GLuint loc;
    /* the handle of the sampler used this texture (given by glGenSamplers). */
    GLuint sampler;
}Texture;

/**
 * A structure for holding the ID's of the shaders associated with a given 
 * material.
 */
typedef struct tagMaterial {
    GLuint frag;
    GLuint vert;
    GLuint geom;
    GLuint program;

    GLuint modelMatrixID;
    GLuint viewMatrixID;
    GLuint projectionMatrixID;

    int numTextures;
    Texture texture;
}Material;

/**
 * Initialize the material system.
 * @return zero on success, nonzero on error.
 */
int Material_Init();

/** 
 * Load the material defined in the given file and return it.
 * The material file is a JSON format file that defines what shaders and 
 * attributes are used in the given material.
 * @param matFile the file containing the various shader and attribute 
 *  definitions.
 * @return the material defined in the given file or NULL in case of error.
 */
Material* Material_Load(const char* matFile);

/** 
 * Compile the given shader of the given shader type.
 * @param shader the source of the shader to compile.
 * @param type the type of the shader e.g. GL_VERTEX_SHADER.
 * @return the compiled shader.
 */
GLuint Material_CompileShader(const GLchar* shader, GLuint type);

/**
 * Compile a program consisting of the given shaders and attributes.
 * @param vertShader the vertex shader of the program to compile.
 * @param fragShader the fragment shader of the program to compile.
 * @param geomShader the geometry shader of the program or NULL for none.
 * @param attributes array of strings for inputs to the vertex shader.
 * @param numAttributes the number of attributes given to the vertex shader.
 * @return the handle to the compiled shader program.
 */
GLuint Material_CompileProgram(GLuint vertShader, GLuint fragShader, GLuint geomShader, 
        char **attributes, int numAttributes);

/** 
 * Add a texture with the given properties to the given material.
 * @param m the material to add the texture to.
 * @param w the width of the texture being added.
 * @param h the height of the texture being added.
 * @param data the texture data in 8 bit RGBA format (see Texel).
 */
void Material_AddTexture(Material* m, int w, int h, Texel* data);

/**
 * Directly set the texture of the given material to the given texture.
 * @param m the material to set the texture of.
 * @param t the texture to set the material's texture to.
 */
void Material_SetTexture(Material* m, Texture* t);

#ifdef __cplusplus
}
#endif
#endif
