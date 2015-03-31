#ifndef GUI_H
#define GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tv_draw.h"
#include "widget.h"
#include "widget_frame.h"
#include "widget_textbox.h"
#include "widget_stats.h"


/** 
 * Get the normalized position of the given X coordinate.
 * @return the x coordinate (in the range 0-1).
 */
static tvfloat    tv_gui_x(tvuint screen_x) {return (tvfloat)(screen_x / screen->w);}
/** 
 * Get the normalized position of the given Y coordinate.
 * @return the y coordinate (in the range 0-1).
 */
static tvfloat    tv_gui_y(tvuint screen_y) {return (tvfloat)(screen_y / screen->h);}
/** 
 * Get the normalized position of the given X and Y coordinates.
 * @return a vector containing the normalized x, y coordinates.
 */
static tv_vector2 tv_gui_xy(tvuint screen_x, tvuint screen_y) {tv_vector2 ret = {((tvfloat)screen_x)/((tvfloat)screen->w), ((tvfloat)screen_y) / ((tvfloat)screen->h)}; return ret;}

#ifdef __cplusplus
}
#endif
#endif