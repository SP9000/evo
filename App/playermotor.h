#ifndef PLAYER_MOTOR_H
#define PLAYER_MOTOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "app.h"

COMPONENT(app_player_motor, tv_component) 
	tv_transform *transform;
	tvfloat speed;
	tv_input_button move_left_button;
	tv_input_button move_right_button;
ENDCOMPONENT(app_player_motor)

#ifdef __cplusplus
extern "C" {
#endif

#endif