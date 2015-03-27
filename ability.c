#include "ability.h"
#include "hud.h"

COMPONENT_NEW(app_ability, tv_component)
	//self->thumbnail.renderer = tv_animation_renderer_new();
END_COMPONENT_NEW(app_ability)

COMPONENT_START(app_ability)
	self->user = (app_unit*)tv_component_get(self_component, app_unit_id());
END_COMPONENT_START

COMPONENT_UPDATE(app_ability)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(app_ability)
END_COMPONENT_DESTROY

void app_ability_set_user(app_ability *ability, app_unit *unit)
{
	ability->user = unit;
}
void app_ability_use_on(app_ability* self, app_ability_target target)
{
	self->use(self, target);
}
void app_ability_use(app_ability* self)
{
	self->use(self, self->target);
}
tvbool app_ability_is_in_range(app_ability* ability, app_ability_target target)
{
	/* if target is one or more units, check if the ability is in range of any */
	if(target.targeted_units != NULL) {
		app_unit** u;
		for(u = (app_unit**)utarray_front(target.targeted_units); u != NULL; u = (app_unit**)utarray_next(target.targeted_units, u)) {
			tv_component* c = *u;
			if(tv_vector3_distance(((tv_component*)(ability->user))->transform->pos, c->transform->pos) <= ability->range) {
				return TRUE;
			}
		}
		return FALSE;
	}
	/* target is point, just check if it's in range */
	return (tv_vector3_distance(((tv_component*)(ability->user))->transform->pos, target.target) <= ability->range);
}

tvbool app_ability_unit_is_in_range(app_ability* ability, app_unit* u) 
{
	return (tv_vector3_distance(((tv_component*)(ability->user))->transform->pos,
		((tv_component*)ability->user)->transform->pos)	<= 
		ability->range);
}

tvbool app_ability_move_user_to_target_range(app_ability *ability)
{
	tv_vector3 pos = ((tv_component*)ability->user)->transform->pos;
	tv_vector3 dir;
	tvfloat speed;

	if(ability->user == NULL) {
		return FALSE;
	}
	/* get the direction and velocity to move toward the target at */
	tv_vector3_direction(ability->target.target, pos, &dir);
	speed = ability->user->current_stats.speed * tv_time_delta;

	/* move the entity */
	if(tv_vector3_distance(pos, ability->target.target) < ability->range) {
		/* target is in range */
		return TRUE;
	}
	/* if there is less than another "step" from the user to the targeting range, 
	 * move the last bit of distance */
	else if(tv_vector3_distance(pos, ability->target.target) < (speed + ability->range)) {
		((tv_component*)ability->user)->transform->pos = ability->target.target;
		return FALSE;
	}
	else {
		/* continue moving toward target. */
		((tv_component*)ability->user)->transform->pos.x += speed * dir.x;
		((tv_component*)ability->user)->transform->pos.y += speed * dir.y;
		((tv_component*)ability->user)->transform->pos.z += speed * dir.z;
		return FALSE;
	}
}

void app_ability_set_target(app_ability *ability, tvfloat x, tvfloat y, tvfloat z)
{
	ability->target.target.x = x;
	ability->target.target.y = y;
	ability->target.target.z = z;
}

void app_ability_set_thumbnail(app_ability *ability, tv_model *model, tv_material *material, tvbool scale)
{
	tv_animation_set_root(ability->thumbnail.animation, model, material);
	if(scale) {
		TvAABB aabb = tv_model_get_aabb(model);
		tv_rect tr = app_hud_thumbnail_dim();
		tv_vector3 scale = {
			tr.w / aabb.w,
			tr.h / aabb.h,
			1.0f
		};
		tv_model_apply_scale(model, scale);
	}
}

