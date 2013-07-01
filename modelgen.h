#if 0
/******************************************************************************
 * modelgen.h
 * This file contains functions to create models of various things. This 
 * includes text, rects, etc.
 *
 * Bryce Wilson
 * created: June 2013
 *****************************************************************************/
#ifndef _MODELGEN_H
#define _MODELGEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "draw.h"
#include "model.h"

/**
 * Create a model of the specified text.
 * @param text the text to generate a model of.
 * @param w the width of the bounds to generate the text inside.
 * @param h the height of the bounds to generate the text inside.
 */
Model* ModelGen_Text(char *text, float w, float h);

/**
 * Create a model of a rectangle using the given position and dimensions.
 * @param x the X coordinate of the bottom left point of the rectangle (from 0-1).
 * @param y the Y coordinate of the bottom left point of the rectangle (from 0-1).
 * @param z the Z coordinate to draw the rectangle at.
 * @param w the width of the rectangle (from 0-1).
 * @param h the height of the rectangle (from 0-1).
 * @return a model of a rectangle of the given parameters.
 */
Model* ModelGen_Rect(float x, float y, float z, float w, float h);

/**
 * Create a model of a box with the given position and dimensions.
 * @param x the X coordinate.
 * @param y the Y coordinate.
 * @param z the Z coordinate.
 * @param w the width of the box.
 * @param h the height of the box.
 * @param d the depth of the box.
 */
Model* ModelGen_Box(float x, float y, float z, float w, float h, float d);

#ifdef __cplusplus
}
#endif

#endif
#endif
