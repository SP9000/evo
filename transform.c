#include "transform.h"

COMPONENT_NEW(tv_transform, tv_component)
	self->pos.x = 0.0f;
	self->pos.y = 0.0f;
	self->pos.z = 0.0f;
	self->rot.x = 0.0f;
	self->rot.y = 0.0f;
	self->rot.z = 0.0f;
	self->scale.x = 1.0f;
	self->scale.y = 1.0f;
	self->scale.z = 1.0f;
END_COMPONENT_NEW(tv_transform)

COMPONENT_START(tv_transform)
END_COMPONENT_START

COMPONENT_UPDATE(tv_transform)
END_COMPONENT_UPDATE
