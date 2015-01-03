#include "app_sine_move.h"

COMPONENT_NEW(APP_sine_move, tv_component)
	self->timer = tv_timer_new(1.0f, TV_TIMER_NO_ROLLOVER, TV_TIMER_NO_RESET);
END_COMPONENT_NEW(APP_sine_move)

COMPONENT_START(APP_sine_move)
END_COMPONENT_START

COMPONENT_UPDATE(APP_sine_move)
	tv_vector3 p;
	tv_timer_update(self->timer);
	p.x = sin(tv_timer_get_time(self->timer)) * self->speed.x;
	p.y = sin(tv_timer_get_time(self->timer)) * self->speed.y;
	p.z = sin(tv_timer_get_time(self->timer)) * self->speed.z - 7.0f;
	self_component->transform->pos = p;
END_COMPONENT_UPDATE

void app_sine_move_set_speed(APP_sine_move* sm, tv_vector3 new_speed)
{
	sm->speed = new_speed;	
}