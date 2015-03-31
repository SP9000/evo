#ifndef PROJECTILE_H
#define PROJECTILE_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "evo.h"

/**
 * A virtual component for projectiles.
 */
COMPONENT(APP_projectile, tv_component)
	tvfloat speed;
	tvfloat range;

	/* the projectile's animation - no sequences required. */
	tv_animation *animation;
	/* the projectile's collider */
	TV_collider *collider;
	/* the SLOT that is activated upon collision */
	TV_message_slot *on_collision;
ENDCOMPONENT(APP_projectile)

#ifdef __cplusplus
}
#endif
#endif