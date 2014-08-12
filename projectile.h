#ifndef PROJECTILE_H
#define PROJECTILE_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "evo.h"

COMPONENT(APP_projectile, tv_component)
	tvfloat speed;
	tvfloat range;
	TV_collider *collider;
ENDCOMPONENT(APP_projectile)

#ifdef __cplusplus
}
#endif
#endif