#ifndef ABILITY_H
#define ABILITY_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

#define APP_ABILITY_MAX_TARGETS 32
#define APP_UNIT_MAX_STATUS_CONDITIONS 16
#define APP_UNIT_MAX_ABILITIES 10

/* the allegiances that a unit may have (1 bit each) */
#define APP_UNIT_ALLEGIANCE_NONE 0x00000000
#define APP_UNIT_ALLEGIANCE_PLAYER		(1 << 0)
#define APP_UNIT_ALLEGIANCE_SWAMPLORD	(1 << 1) 
#define APP_UNIT_ALLEGIANCE_DOOMSPHERE	(1 << 2)

/* alliances that a unit may be loyal to (convienience OR'ing of allegiances) */
#define APP_UNIT_ALLIANCE_NONE 0x00000000
#define APP_UNIT_ALLIANCE_PLAYER_ONLY (APP_UNIT_ALLEGIANCE_PLAYER)
#define APP_UNIT_ALLIANCE_DOOMSWAMP	  (APP_UNIT_ALLEGIANCE_SWAMPLORD | APP_UNIT_ALLEGIANCE_DOOMSPHERE)

/******************************************************************************/
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
 * The types of damage that may be dealt by an ability.
 */
typedef enum {
	APP_ABILITY_DAMAGE_TYPE_PHYSICAL,
	APP_ABILITY_DAMAGE_TYPE_MAGICKAL
}app_ability_damage_type;

/**
 * A structure containing information about damage that is dealt by an ability.
 */
typedef struct app_ability_damage {
	app_ability_damage_type type;	/**< the type of the damage */
	tvfloat amount;					/**< the base amount of the damage */
}app_ability_damage;

/**
 * The ability component.
 */
COMPONENT(app_ability, tv_component)
	/* the animation of this ability's user */
	tv_animation* animation;
	/* the animation loop to play when this ability is used */
	tvuint animation_use_loop;
	/* the delay from beginning to play the animation to the ability's effect */
	tvfloat cast_time;

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

	/* a flag register of up to 32 units this unit has allegiance to. */
	tvuint ally_allegiances;
	/* a flag register of up to 32 units this unit is hostile toward. */
	tvuint enemy_allegiances;

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
void app_ability_set_user(app_ability *ability, app_unit *unit);

/**
 * Use the given ability.
 */
void app_ability_use(app_ability* ability);

/**
 * Checks if the given target is within the "use" range of the given ability.
 * @param ability the ability to test.
 * @param target the target to determine if is in range.
 * @return TRUE if the ability is in range, FALSE if not.
 */
tvbool app_ability_target_is_in_range(app_ability* ability, app_ability_target target);
/**
 * Checks if the given unit is within the "use" range of the given ability.
 * @param ability the ability to test.
 * @param target the target to determine if is in range.
 * @return TRUE if the ability is in range, FALSE if not.
 */
tvbool app_ability_unit_is_in_range(app_ability* ability, app_unit* unit);

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

/** 
 * Deal the given damage to the given unit.
 * @param unit the unit to take the damage.
 * @param damage the damage the unit is to take. 
 */
void app_unit_take_damage(app_unit* unit, app_ability_damage damage);

/**
 * Sets the allegiances of the given unit.
 * @param unit the unit to set the allegiances of.
 * @param allegiances the allegiances this unit is friendly toward.
 */
void app_unit_set_allied_allegiances(app_unit* unit, tvuint allegiances);
void app_unit_set_enemy_allegiances(app_unit* unit, tvuint allegiances);

void app_unit_add_allied_allegiances(app_unit* unit, tvuint allegiances);
void app_unit_remove_allied_allegiances(app_unit* unit, tvuint allegiances);

void app_unit_add_enemy_allegiances(app_unit* unit, tvuint allegiances);
void app_unit_remove_enemy_allegiances(app_unit* unit, tvuint allegiances);

#ifdef __cplusplus
}
#endif
#endif