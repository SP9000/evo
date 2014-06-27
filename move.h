#ifndef MOVE_H
#define MOVE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "ability.h"

COMPONENT(app_move, app_ability)
	tvfloat speed;
ENDCOMPONENT(app_move)

#ifdef __cplusplus
}
#endif
#endif