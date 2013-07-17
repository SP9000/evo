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

#ifdef __cplusplus
}
#endif

#endif
