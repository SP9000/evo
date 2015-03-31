#include "move.h"

static void use(app_ability *self, app_ability_target target);

COMPONENT_NEW(app_move, app_ability)
	app_ability* ability = (app_ability*)self;
	ability->use = use; 
	ability->range = 0.0f;
END_COMPONENT_NEW(app_move)

COMPONENT_START(app_move)
	((app_ability*)self)->user = (app_unit*)tv_component_get(self_component, app_unit_id());
END_COMPONENT_START

COMPONENT_UPDATE(app_move)
	/* all we need to do is move toward the target */
	app_ability_move_user_to_target_range((app_ability*)self);
END_COMPONENT_UPDATE
COMPONENT_DESTROY(app_move)
	/* all we need to do is move toward the target */
	app_ability_move_user_to_target_range((app_ability*)self);
END_COMPONENT_DESTROY

void use(app_ability *self, app_ability_target target)
{
	app_ability* ability = (app_ability*)self;
	ability->target = target;
}