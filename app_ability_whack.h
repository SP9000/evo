#ifndef APP_ABILITY_WHACK_H
#define APP_ABILITY_WHACK_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "ability.h"

COMPONENT(app_ability_whack, app_ability)
	/* the ability to use to move the user of this ability to the target */
	app_ability* move_to;
	app_ability_damage damage;	/**< the damage that "whack" deals */
ENDCOMPONENT(app_ability_whack)

#ifdef __cplusplus
}
#endif
#endif
