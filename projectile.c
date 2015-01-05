#include "projectile.h"

static void col(TV_message *msg);

COMPONENT_NEW(APP_projectile, tv_component)
END_COMPONENT_NEW(APP_projectile)

COMPONENT_START(APP_projectile)
	tv_component_require(self_component, TV_collider_id());
	self->collider = (TV_collider*)tv_component_get(self_component, TV_collider_id());
	tv_message_connect(self->on_collision, self->collider->on_collision, col);

END_COMPONENT_START

COMPONENT_DESTROY(APP_projectile)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(APP_projectile)
END_COMPONENT_UPDATE

void col(TV_message *msg)
{
	puts("COLLISION");
}