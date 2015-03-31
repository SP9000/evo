/*****************************************************************************/
/* This file contains convienient initializers for various abilities.		 */
/* Created Jun 9, 2014.														 */
/* Bryce Wilson																 */
/*****************************************************************************/
#ifndef ABILITIES_H
#define ABILITIES_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "app_ability_move.h"

/**
 * Creates and initializes a move component 
 */
tv_entity *ability_move(tv_entity *user);

#ifdef __cplusplus
}
#endif
#endif