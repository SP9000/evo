#include "tv_light.h"

COMPONENT_NEW(tv_light, tv_component)
	self->color = tv_vector4_zero;
	self->intensity = 1.0f;
	self->range = 10.0f;
END_COMPONENT_NEW(tv_light)

COMPONENT_START(tv_light)
END_COMPONENT_START

COMPONENT_UPDATE(tv_light)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(tv_light)
END_COMPONENT_DESTROY