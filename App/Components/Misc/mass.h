#ifndef MASS_H
#define MASS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

COMPONENT(APP_mass, tv_component)
	tvfloat kgs;
ENDCOMPONENT(APP_mass)

/**
 * Add the given number of kg's to the given mass component.
 * @param mass the mass to add the given amount of mass to.
 * @param kgs the number of kg's to add.
 */
void app_mass_add(APP_mass *mass, tvfloat kgs);
/**
 * Remove the given number of kg's from the given mass component.
 * @param mass the mass to remove.
 * @param kgs the number of kg's to add.
 */
void app_mass_remove(APP_mass *mass, tvfloat kgs);

/**
 * Get the number of kg's attached to the given mass component.
 */
static tvfloat app_mass_get(APP_mass *mass) {return mass->kgs;}

#ifdef __cplusplus
}
#endif
#endif