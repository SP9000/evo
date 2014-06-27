#ifndef MELEE_H
#define MELEE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "ability.h"

COMPONENT(app_melee, app_ability)
	/* the distance from the target that the melee becomes effective. */
	tvfloat attack_range;
	tvfloat speed;
ENDCOMPONENT(app_melee)

#ifdef __cplusplus
}	
#endif
#endif