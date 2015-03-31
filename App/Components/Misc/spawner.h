#ifndef SPAWNER_H
#define SPAWNER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

COMPONENT(APP_spawner, tv_component)
	/* the time that must pass before this spawner produces another burst */
	tvfloat cooldown;
	/* the remaining time before another enemy spawns again */
	tvfloat curr_cooldown;

	/* a reference to the function to call on spawn */
	tv_entity* (*spawn_func)();

	/* the rate that new entities are spawned at. */
	tvfloat spawn_rate;
	/* the delay between bursts of enemies. */
	tvfloat burst_delay;

	/* the # of entities to spawn per burst */
	tvuint entities_per_burst;
	/* the # of bursts this spawner produces. */
	tvuint num_bursts;
	/* the # of the current entity (within the burst) */
	tvuint entity_num;
	/* the # of the current burst. */
	tvuint burst_num;
ENDCOMPONENT(APP_spawner)

void app_spawner_set_spawn_func(APP_spawner *spawner, tv_entity* (*f)());
/**
 * Update spawner timers and determine if another entity should be spawned.
 * @param spawner the spawner to update.
 * @return TRUE if another entity should be spawned, else FALSE.
 */
tvbool app_spawner_tick(APP_spawner *spawner);

#ifdef __cplusplus
}
#endif
#endif