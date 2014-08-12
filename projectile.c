#include "projectile.h"

COMPONENT_NEW(APP_projectile, tv_component)
END_COMPONENT_NEW(APP_projectile)

COMPONENT_START(APP_projectile)
	tv_component_require(self_component, TV_collider_id());
	self->collider = (TV_collider*)tv_component_get(self_component, TV_collider_id());
END_COMPONENT_START

COMPONENT_UPDATE(APP_projectile)
END_COMPONENT_UPDATE
