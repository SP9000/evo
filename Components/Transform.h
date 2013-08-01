/*****************************************************************************/
/* Transform.c                                                               */
/* The transform component - common to all entities.                         */
/* The transform component contains functions to manipulate a entities'		 */
/* transform in the scene. The data it manipulates is held in the entity     */
/* itself																	 */
/* Bryce Wilson                                                              */
/* Created: June, 2013                                                       */
/*****************************************************************************/
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "All.h"
typedef struct Transform;
typedef struct Transform {
	TvComponent base;
}Transform;

#endif