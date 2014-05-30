#ifndef TERRAINCURSOR_H
#define TERRAINCURSOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "App\app.h"
#include "terrain.h"

#define APP_TERRAIN_CURSOR_SPEED 0.20f

COMPONENT(app_terrain_cursor, tv_component) 
	app_terrain *terrain;
	tv_model *model;
	tv_model_renderer *renderer;
	tv_input_button move_left_key;
	tv_input_button move_right_key;
	tv_input_button move_up_key;
	tv_input_button move_down_key;
	tv_input_button move_forward_key;
	tv_input_button move_back_key;
	tv_input_button select_key;
	tvfloat x_step;
	tvfloat y_step;
	tvfloat z_step;
	tv_timer timer;
ENDCOMPONENT(app_terrain_cursor)

#ifdef __cplusplus
extern "C" {
#endif

#endif