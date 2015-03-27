#include "ability.h"
#include "app_ability_move.h"

COMPONENT_NEW(app_unit, tv_component)
	self->float_height = 0.0f;
	self->ally_allegiances  = APP_UNIT_ALLEGIANCE_NONE;
	self->enemy_allegiances = APP_UNIT_ALLEGIANCE_NONE;
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

void app_unit_set_move_ability(app_unit* unit, app_ability* move_ability)
{
	unit->move_to = move_ability;
}

void app_unit_take_damage(app_unit* unit, app_ability_damage damage)
{
	/* TODO: implement actual stats based damage calculation */
	unit->current_stats.hp -= damage.amount;
}

void app_unit_set_allied_allegiances(app_unit* unit, tvuint allegiances)
{
	unit->ally_allegiances = allegiances;
}
void app_unit_set_enemy_allegiances(app_unit* unit, tvuint allegiances)
{
	unit->enemy_allegiances = allegiances;
}
void app_unit_add_allied_allegiances(app_unit* unit, tvuint allegiances)
{
	unit->ally_allegiances |= allegiances;
}
void app_unit_remove_allied_allegiances(app_unit* unit, tvuint allegiances)
{
	unit->ally_allegiances &= ~(allegiances);
}
void app_unit_add_enemy_allegiances(app_unit* unit, tvuint allegiances)
{
	unit->enemy_allegiances |= allegiances;
}
void app_unit_remove_enemy_allegiances(app_unit* unit, tvuint allegiances)
{
	unit->enemy_allegiances &= ~(allegiances);
}