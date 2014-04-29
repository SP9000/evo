/*****************************************************************************/
/* widgetmodels.h                                                            */
/* This file contains definitions for functions that create models for use in*/
/* the GUI.	 All the generated models are produced in "unit" dimensions      */
/* unless otherwise specified.												 */
/*                                                                           */
/* TODO: could probably make all these functions virtual and put all this    */
/*   in a component so each widget has control over where it gets its models */
/* Bryce Wilson                                                              */
/* created: April 28, 2014                                                   */
/*****************************************************************************/
#ifndef WIDGETMODELS_H
#define WIDGETMODELS_H

#include "model.h"

/** 
 * Get a model of the given text in the given bounds.
 * @param text the text to produce the model of.
 * @param bounds the rectangle to fit the text within.
 * @return a reference to the created model.
 */
tv_model *tv_gui_model_text(tvchar *text, tv_rect bounds);

/**
 * Get a model of a simple border.
 * @return a model representing the border of a widget.
 */
tv_model *tv_gui_model_border(tvfloat w, tvfloat h, tv_vector4 color);

#endif