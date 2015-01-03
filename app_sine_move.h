#ifndef APP_SINE_MOVE_H
#define APP_SINE_MOVE_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "evo.h"

/**
 * A virtual component for projectiles.
 */
COMPONENT(APP_sine_move, tv_component)
	tv_vector3 speed;
	tv_timer* timer;
ENDCOMPONENT(APP_sine_move)


void app_sine_move_set_speed(APP_sine_move* sm, tv_vector3 new_speed);

#ifdef __cplusplus
}
#endif
#endif