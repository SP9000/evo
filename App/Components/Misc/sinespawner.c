#include "sinespawner.h"

COMPONENT_NEW(APP_sine_spawner, APP_spawner)
	APP_spawner *s = (APP_spawner*)self;
	s->burst_num = 0;
	s->entity_num = 0;
	s->curr_cooldown = 0.0f;
	self->amplitude.x = self->amplitude.y = self->amplitude.z = 1.0f;
END_COMPONENT_NEW(APP_sine_spawner)

COMPONENT_START(APP_sine_spawner)
	APP_spawner *s = (APP_spawner*)self;
END_COMPONENT_START

COMPONENT_DESTROY(APP_sine_spawner)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(APP_sine_spawner)
	APP_spawner *s = (APP_spawner*)self;
	tv_entity *e;
	if(app_spawner_tick(s)) {
		tv_vector3 v = {0,0,0};
		v.x = self->amplitude.x * sin(self->frequency.x * (tvfloat)s->entity_num);
		v.y = self->amplitude.y * sin(self->frequency.y * (tvfloat)s->entity_num);
		v.z = self->amplitude.z * sin(self->frequency.z * (tvfloat)s->entity_num);
		e = s->spawn_func();
		e->transform.pos = v;
	}
END_COMPONENT_UPDATE

