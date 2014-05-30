#ifndef UNIT_H
#define UNIT_H
#ifdef __cplusplus
extern "C" {
#endif

#define APP_UNIT_MAX_STATUS_CONDITIONS 16
#define APP_UNIT_MAX_ABILITIES 10

#include "evo.h"
#include "ability.h"


typedef struct app_unit_stats {
	tvint hp;
	tvint magick;
	tvint strength;
	tvint defense;
	tvint speed;
}app_unit_stats;

typedef struct app_unit_status_condition {
	tvfloat duration;
	tvfloat time_left;
	tvint strength;
	void (*update)(struct app_unit *unit);
}app_unit_status_condition;

COMPONENT(app_unit, tv_component)
	tv_model *model;
	app_unit_stats base_stats;
	app_unit_stats current_stats;
	app_unit_status_condition conditions[APP_UNIT_MAX_STATUS_CONDITIONS];

	tvuint num_abilities;
	app_ability abilities[APP_UNIT_MAX_ABILITIES];
ENDCOMPONENT(app_unit)

void app_unit_apply_status(app_unit *unit, app_unit_stats *status);
void app_unit_remove_status(app_unit *unit, app_unit_stats *status);
void app_unit_add_ability(app_unit *unit, app_ability *ability);
void app_unit_remove_ability(app_unit *unit, app_ability *ability);

#ifdef __cplusplus
}
#endif
#endif