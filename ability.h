#ifndef ABILITY_H
#define ABILITY_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#define APP_ABILITY_MAX_TARGETS 32

typedef enum  {
	APP_ABILITY_SINGLE_TARGET,
	APP_ABILITY_RADIUS_TARGET,
	APP_ABILITY_LINE_TARGET
}app_ability_target_type;

typedef struct app_ability_target {
	/* the position of the target */
	tv_vector3 target;
	/* the targeted units */
	tv_array *targeted_units;
}app_ability_target;

COMPONENT(app_ability, tv_component)
	tv_model *model;
	tvfloat range;
	tvfloat cooldown;
	tvfloat curr_cooldown;

	app_ability_target_type target_type;
	app_ability_target target;
ENDCOMPONENT(app_ability)

#ifdef __cplusplus
}
#endif
#endif