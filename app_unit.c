#include "ability.h"
#include "app_ability_move.h"

COMPONENT_NEW(app_unit, tv_component)
	self->float_height = 0.0f;
END_COMPONENT_NEW(app_unit)

COMPONENT_START(app_unit)
	self->animation = (tv_animation*)tv_component_get(self_component, tv_animation_id());
END_COMPONENT_START

COMPONENT_UPDATE(app_unit)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(app_unit)
END_COMPONENT_DESTROY

void app_unit_move_to(app_unit* unit, app_ability_target* target)
{
	tvfloat x, y, z;
	x = target->target.x;
	y = target->target.y;
	z = target->target.z;

	app_ability_set_target(unit->move_to, x, y + unit->float_height, z);
}

void METHOD(app_unit, set_float_height, tvfloat height)
{
	self->float_height = height;
}