#include "gun.h"

COMPONENT_NEW(APP_gun, tv_component)
END_COMPONENT_NEW(APP_gun)

COMPONENT_START(APP_gun)
END_COMPONENT_START

COMPONENT_UPDATE(APP_gun)
	self->curr_cooldown -= tv_time_delta;
	if(tv_input_buttonpressed(self->fire_button) && self->curr_cooldown <= 0.0f) {
		self->projectile->speed = 10.0f;
		self->curr_cooldown = self->cooldown;
	}
END_COMPONENT_UPDATE

COMPONENT_DESTROY(APP_gun)
END_COMPONENT_DESTROY