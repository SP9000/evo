#include "velocity.h"

void app_velocity_add(APP_velocity *vel, tv_vector3 mps)
{
	 tv_vector3_add(vel->mps, mps, &vel->mps);
}
void app_velocity_remove(APP_velocity *vel, tv_vector3 mps)
{
	tv_vector3_add(vel->mps, mps, &vel->mps);
}

COMPONENT_NEW(APP_velocity, tv_component)
END_COMPONENT_NEW(APP_velocity)

COMPONENT_START(APP_velocity)
END_COMPONENT_START

COMPONENT_DESTROY(APP_velocity)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(APP_velocity)
	/* move the entity's transform by an amount of the velocity */
	self_component->transform->pos = (self_component->transform->pos, self->mps);
END_COMPONENT_UPDATE
