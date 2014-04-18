#include "playermotor.h"

COMPONENT_NEW(app_player_motor, tv_component)
	self->speed = 0.0f;
END_COMPONENT_NEW(app_player_motor)

COMPONENT_START(app_player_motor)
	self->transform = (tv_transform*)tv_component_get(self_component, tv_transform_id());
END_COMPONENT_START

COMPONENT_UPDATE(app_player_motor)
	if(tv_input_keydown(INPUT_KEY_RIGHT)) {
		self_component->entity->transform.pos.x -= self->speed * tv_time_delta;
	}
	if(tv_input_keydown(INPUT_KEY_LEFT)) {
		self_component->entity->transform.pos.x += self->speed * tv_time_delta;
	}
END_COMPONENT_UPDATE
