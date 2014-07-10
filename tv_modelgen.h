/*****************************************************************************/
/* modelgen.h                                                                */
/* This file contains functions to create .PLY files for models of various   */
/* things. This includes the character set, rects, etc.                      */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: June 2013                                                        */
/*****************************************************************************/
#ifndef _MODELGEN_H
#define _MODELGEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "model.h"


/**
 * Generate models for the character set.
 * Models are output to ./StdAssets/Charset/
 */
void ModelGen_Charset();

/**
 * Generate various default models used by the GUI (scrollbars, buttons, etc.)
 * Models are output to ./StdAssets/GUI/
 */
void ModelGen_GUI();

/**
 * Returns a tetrahedron of the given dimensions.
 * @param dimensions the (w,h,d) dimensions of the tetrahedron.
 * @param format the format of the vertices for this tetrahedron.
 * @return the tetrahedron.
 */
tv_model *tv_modelgen_tetrahedron(tv_vector3 dimensions, tv_model_vertex format);

/**
 * Returns a quad of the given dimensions.
 * @param dimensions the (w,h) dimensions of the quad.
 * @param format the format for the vertices of the quad.
 * @return the quad.
 */
tv_model *tv_modelgen_quad(tv_vector2 dimensions, tv_model_vertex format);

#ifdef __cplusplus
}
#endif

#endif
