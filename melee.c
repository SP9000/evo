#include "melee.h"

static void use(app_ability *self, app_ability_target target);

COMPONENT_NEW(app_melee, app_ability)
	app_ability* ability = (app_ability*)self;
	ability->use = use; 
END_COMPONENT_NEW(app_melee)

COMPONENT_START(app_melee)
	((app_ability*)self)->user = (app_unit*)tv_component_get(self_component, app_unit_id());
END_COMPONENT_START

COMPONENT_UPDATE(app_melee)
	/* move into target range */
	if(app_ability_move_user_to_target_range((app_ability*)self)) {
		/* perform the melee attack */

	}
END_COMPONENT_UPDATE

void use(app_ability *self, app_ability_target target)
{
	app_ability* ability = (app_ability*)self;
	ability->target = target;
}