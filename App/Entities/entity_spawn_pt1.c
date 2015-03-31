#include "app_entities.h"

ENTITY(spawn_pt, 0,0,0, 1,1,1, 0,0,0)
ADDCOMPONENT(APP_sine_spawner, spawner)
	APP_spawner *s = (APP_spawner*)spawner;
	app_spawner_set_spawn_func(s, fairy_prefab);
	spawner->amplitude.x = 1.0f;
	spawner->amplitude.y = 1.0f;
	spawner->amplitude.z = 1.0f;
	spawner->frequency.x = 1.0f;
	spawner->frequency.y = 1.0f;
	spawner->frequency.z = 1.0f;
	s->spawn_rate = 0.75f;
	s->burst_delay = 2.0f;
	s->entities_per_burst = 2;
	s->num_bursts = 12;
ENTITY_END
