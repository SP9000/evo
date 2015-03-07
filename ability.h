#ifndef ABILITY_H
#define ABILITY_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

#define APP_ABILITY_MAX_TARGETS 32
#define APP_UNIT_MAX_STATUS_CONDITIONS 16
#define APP_UNIT_MAX_ABILITIES 10

/*****************************************************************************/
/**
 * An enumeration of possible targeting methods.
 */
typedef enum  {
	APP_ABILITY_SINGLE_TARGET,
	APP_ABILITY_RADIUS_TARGET,
	APP_ABILITY_LINE_TARGET
}app_ability_target_type;

/** 
 * A structure that defines an ability's target.
 */
typedef struct app_ability_target {
	/* the position of the target */
	tv_vector3 target;
	/* the targeted units */
	tv_array *targeted_units;
}app_ability_target;

typedef struct app_ability_thumbnail {
	/* the model of this ability's thumbnail */
	tv_animation *animation;
	/* the thumbnail renderer */
	tv_animation_renderer *renderer;
}app_ability_thumbnail;

/**
 * The ability component.
 */
COMPONENT(app_ability, tv_component)
	/* the unit that this ability is attached to */
	struct app_unit *user;
	/* the range at which this ability can be used on a target */
	tvfloat range;
	/* the time that must elapse before this ability may be used again */
	tvfloat cooldown;
	/* the current amount of time left before this ability can be reused */
	tvfloat curr_cooldown;

	/* the thumbnail for this ability */
	app_ability_thumbnail thumbnail;

	/* the type of targeting that this ability uses */
	app_ability_target_type target_type;
	/* the current target of this ability */
	app_ability_target target;
	/* the function that is called when this ability is used */
	void (*use)(struct app_ability *self, app_ability_target target);
ENDCOMPONENT(app_ability)

/*****************************************************************************/
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
	tv_animation *animation;
	app_unit_stats base_stats;
	app_unit_stats current_stats;
	app_unit_status_condition conditions[APP_UNIT_MAX_STATUS_CONDITIONS];

	/* the offset above the center of the unit that this unit "floats" */
	tvfloat float_height;

	/* every unit has a "move to" ability */
	app_ability* move_to;

	tvuint num_abilities;
	app_ability abilities[APP_UNIT_MAX_ABILITIES];
ENDCOMPONENT(app_unit)

/*****************************************************************************/
void app_unit_apply_status(app_unit *unit, app_unit_stats *status);
void app_unit_remove_status(app_unit *unit, app_unit_stats *status);
void app_unit_add_ability(app_unit *unit, app_ability *ability);
void app_unit_remove_ability(app_unit *unit, app_ability *ability);
void app_unit_set_move_ability(app_unit* unit, app_ability* move_ability);
void app_unit_move_to(app_unit* unit, app_ability_target* target);
void METHOD(app_unit, set_float_height, tvfloat height);
/*****************************************************************************/
/**
 * Set the user of the given ability to the given unit.
 * @param ability the ability to set the user of.
 * @param unit the unit to set the given ability's user property to.
 */
/*
void app_ability_set_user(app_ability *ability, app_unit *unit);
*/

/**
 * Move the user of the given ability within targetting range of the ability.
 * @param ability the ability containing the unit to move and the target to 
 *		move it into range of.
 * @return returns true if the target is within targetting range.
 */
tvbool app_ability_move_user_to_target_range(app_ability *ability);
/**
 * Set the positional target of the given ability to the given coordinates.
 * @param ability the ability to set the target of.
 * @param x the x coordinate to set the target to.
 * @param y the y coordinate to set the target to.
 * @param z the z coordinate to set the target to.
 */
void app_ability_set_target(app_ability *ability, tvfloat x, tvfloat y, tvfloat z);
/**
 * For simple thumbnails (one model and material), this function can be used to
 * set the model and material of the given ability's thumbnail.
 * @param ability the ability to set the thumbnail of.
 * @param model the model to set the thumnail to.
 * @param material the material to render the given model with.
 * @param scale if TRUE, scale the model so it will fit perfectly in the 
 *  thumbnail region.
 */
void app_ability_set_thumbnail(app_ability *ability, tv_model *model, tv_material *material, tvbool scale);
#ifdef __cplusplus
}
#endif
#endif