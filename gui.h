#ifndef GUI_H
#define GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "draw.h"
#include "widget.h"
#include "widget_frame.h"
#include "widget_textbox.h"
#include "widget_stats.h"

static tvfloat tv_gui_x(tvuint screen_x) {return (tvfloat)(screen_x / screen->w);}
static tvfloat tv_gui_y(tvuint screen_y) {return (tvfloat)(screen_y / screen->h);}
static tv_vector2 tv_gui_xy(tvuint screen_x, tvuint screen_y) {tv_vector2 ret = {((tvfloat)screen_x)/((tvfloat)screen->w), ((tvfloat)screen_y) / ((tvfloat)screen->h)}; return ret;}

#ifdef __cplusplus
}
#endif
#endif