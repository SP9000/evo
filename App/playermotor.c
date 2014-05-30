#include "playermotor.h"

COMPONENT_NEW(app_player_motor, tv_component)
	self->speed = 0.0f;	
	self->move_left_button.type = TV_INPUT_KEYBOARD;
	self->move_right_button.type = TV_INPUT_KEYBOARD;
	self->move_left_button.button = INPUT_KEY_LEFT;
	self->move_right_button.button = INPUT_KEY_RIGHT;
END_COMPONENT_NEW(app_player_motor)

COMPONENT_START(app_player_motor)
	self->transform = (tv_transform*)tv_component_get(self_component, tv_transform_id());
END_COMPONENT_START

COMPONENT_UPDATE(app_player_motor)
	if(tv_input_buttondown(self->move_right_button)) {
		self_component->entity->transform.pos.x -= self->speed * tv_time_delta;
	}
	if(tv_input_buttondown(self->move_left_button)) {
		self_component->entity->transform.pos.x += self->speed * tv_time_delta;
	}
END_COMPONENT_UPDATE
