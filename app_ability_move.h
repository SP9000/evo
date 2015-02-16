#ifndef APP_ABILITY_MOVE_H
#define APP_ABILITY_MOVE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "ability.h"

COMPONENT(app_ability_move, app_ability)
	tvfloat speed;
ENDCOMPONENT(app_ability_move)

#ifdef __cplusplus
}
#endif
#endif