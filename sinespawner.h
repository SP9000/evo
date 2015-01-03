#ifndef SINESPAWNER_H
#define SINESPAWNER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "spawner.h"

COMPONENT(APP_sine_spawner, APP_spawner)
	/* the amplitudes of the sinus in each dimension */
	tv_vector3 amplitude;
	/* the frequencies of the sinus in each dimension */
	tv_vector3 frequency;
ENDCOMPONENT(APP_sine_spawner)

#ifdef __cplusplus
}
#endif
#endif
