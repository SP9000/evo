#include "spawner.h"

COMPONENT_NEW(APP_spawner, tv_component)
END_COMPONENT_NEW(APP_spawner)

COMPONENT_START(APP_spawner)
	self->burst_num = 0;
	self->entity_num = 0;
	self->curr_cooldown = 0.0f;
END_COMPONENT_START

COMPONENT_UPDATE(APP_spawner)
END_COMPONENT_UPDATE


void app_spawner_set_spawn_func(APP_spawner *spawner, tv_entity* (*f)())
{
	spawner->spawn_func = f;
}

tvbool app_spawner_tick(APP_spawner *spawner)
{
	/* check that this spawner is still active */
	if(spawner->burst_num > spawner->num_bursts) {
		return FALSE;
	}

	spawner->curr_cooldown -= tv_time_delta;
	/* time to spawn a new entity? */
	if(spawner->curr_cooldown <= 0.0f) {
		++spawner->entity_num;
		/* if we've reached the end of a burst, delay the timer by the burst delay */
		if(!(spawner->entity_num % spawner->entities_per_burst)) {
			spawner->curr_cooldown = spawner->curr_cooldown + spawner->burst_delay;
			++spawner->burst_num;
		}
		else {
			spawner->curr_cooldown = spawner->curr_cooldown + spawner->spawn_rate;
		}
		return TRUE;
	}

	/* not time to spawn an entity */
	else {
		return FALSE;
	}
}
