#ifndef VELOCITY_H
#define VELOCITY_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

COMPONENT(APP_velocity, tv_component)
	/* the velocity in m/s */
	tv_vector3 mps;
ENDCOMPONENT(APP_velocity)

void app_velocity_add(APP_velocity *vel, tv_vector3 mps);
void app_velocity_remove(APP_velocity *vel, tv_vector3 mps);
static tv_vector3 app_velocity_get(APP_velocity *vel) {return vel->mps;}

#ifdef __cplusplus
}
#endif
#endif