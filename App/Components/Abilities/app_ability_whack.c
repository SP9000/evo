#include "app_ability_whack.h"

/* ability use handler */
static void use(app_ability *self, app_ability_target target);

COMPONENT_NEW(app_ability_whack, app_ability)
	app_ability* ability = (app_ability*)self;

	app_ability_init((app_ability*)self);
	ability->use = use; 
	ability->range = 1.0f;
END_COMPONENT_NEW(app_ability_whack)

COMPONENT_START(app_ability_whack)
	((app_ability*)self)->user = (app_unit*)tv_component_get(self_component, app_unit_id());
END_COMPONENT_START

COMPONENT_UPDATE(app_ability_whack)
	app_ability* a = (app_ability*)self;
	/* move to the target first (if not in range) */
	app_ability_use(a->user->move_to);
	/* try to attack the target (if there is one and it's in range) */
	if(a->target.targeted_units) {
		app_ability_target t;
		/* get the unit to attempt the whack on */
		app_unit* u = *((app_unit**)utarray_front(a->target.targeted_units));
		/* if the unit is in range, have it take damage */
		if(app_ability_unit_is_in_range(a, u)) {
			app_unit_take_damage(u, self->damage);
		}
	}
END_COMPONENT_UPDATE

COMPONENT_DESTROY(app_ability_whack)
END_COMPONENT_DESTROY

void use(app_ability *self, app_ability_target target)
{
	app_ability* ability = (app_ability*)self;
	ability->target = target;
}
