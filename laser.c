#include "laser.h"

static void col(TV_message *msg);

COMPONENT_NEW(APP_laser, tv_component)
END_COMPONENT_NEW(APP_laser)

COMPONENT_START(APP_laser)
	APP_projectile *p = (APP_projectile*)self;
	tv_component_require(self_component, TV_collider_id());
	p->collider = (TV_collider*)tv_component_get(self_component, TV_collider_id());
	tv_message_connect(p->on_collision, p->collider->on_collision, col);
END_COMPONENT_START

COMPONENT_UPDATE(APP_projectile)
END_COMPONENT_UPDATE

void col(TV_message *msg)
{
	puts("LAZOZR COLLISION");
}