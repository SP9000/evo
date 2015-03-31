#ifndef GUN_H
#define GUN_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "projectile.h"

COMPONENT(APP_gun, tv_component)
	/* the mass of the gun in kg */
	tvfloat kg;
	/* the button that is used to trigger firing of this gun. */
	tv_input_button fire_button;

	/* the time that must pass before this gun spawns another projectile */
	tvfloat cooldown;
	/* the remaining time before this gun may fire again */
	tvfloat curr_cooldown;
	/* a reference to the type of projectile this gun fires */
	APP_projectile *projectile;
ENDCOMPONENT(APP_gun)

#ifdef __cplusplus
}
#endif
#endif