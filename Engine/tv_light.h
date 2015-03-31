/*****************************************************************************/
/* tv_light.h                                                                */
/* This file defines the basic light component that is used to render lit    */
/* objects by the material components.									     */
/* Bryce Wilson                                                              */
/* Created: April 30, 2014                                                   */
/*****************************************************************************/
#ifndef TV_LIGHT_H
#define TV_LIGHT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tv_component.h"

COMPONENT(tv_light, tv_component)
	tv_vector4 color;
	tv_vector3 dir;
	tvfloat intensity;
	tvfloat range;
ENDCOMPONENT(tv_light)

#ifdef __cplusplus
}
#endif
#endif