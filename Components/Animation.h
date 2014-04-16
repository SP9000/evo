/*****************************************************************************/
/* Animation.h																 */
/* The animation component.                                                  */
/* The animation component contains the data to represent a group of model's */
/* as a heirarchal skeleton and to move this skeleton and the associated     */
/* models.                                                                   */
/* Since models already have some sort of heirarchy via subgroups, this      */
/* component stores just the root model, and you must access deeper levels   */
/* on a case-by-case basis.                                                  */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created July 8, 2013                                                      */
/*****************************************************************************/
#ifndef APP_ANIMATION_H
#define APP_ANIMATION_H
#include "All.h"

typedef struct AnimationCmd;
typedef struct Animation;
/**
 * Animation command structure.
 * The basic commands are: ANIM_TRANSLATE, ANIM_SCALE, and ANIM_ROTATE.
 */
typedef struct AnimationCmd {
    /* command to perform */
    int command;

    /* model to serve as root for this operation (all subgroups of model will
     * also have this operation applied). */
    TvModel* base;

    /* amount to scale/translate/rotate by */
    TvVector3 value;
}AnimationCmd;

typedef struct Animation {
	TvComponent base;
	PUBLIC struct Renderer* root;
	TvArray** animations;
}Animation;
#endif