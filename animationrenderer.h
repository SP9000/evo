/*****************************************************************************/
/*This file contains definitions for the animation renderer component.       */
/* Materials that can be animated must contain a uniform for the bones of the*/
/* animation e.g. uniform mat4 bones[10];								     */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: Jun 11, 2014                                                     */
/*****************************************************************************/

#ifndef ANIMATIONRENDERER_H
#define ANIMATIONRENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "animation.h"
#include "renderer.h"

HANDLER(tv_animation_renderer, tv_renderer)
	tv_animation *animation;
ENDHANDLER(tv_animation_renderer)

void tv_animation_renderer_set_animation(tv_animation_renderer *self, tv_animation *animation);


#ifdef __cplusplus
}
#endif
#endif