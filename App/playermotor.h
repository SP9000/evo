#ifndef PLAYER_MOTOR_H
#define PLAYER_MOTOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "app.h"

COMPONENT(app_player_motor, tv_component) 
	tv_transform *transform;
	tvfloat speed;
ENDCOMPONENT(app_player_motor)

#ifdef __cplusplus
extern "C" {
#endif

#endif