#ifndef JET_ENGINE_H
#define JET_ENGINE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "mass.h"

COMPONENT(APP_jet_engine, tv_component)
	/* the mass of this engine in kg's */
	tvfloat kg;
	/* the maximum power this engine can provide */
	tvfloat max_power;
	/* the engine's acceleration TODO: replace with parametric curve */
	tvfloat acceleration;
	/* the mass component of the entity this engine belongs to. */
	APP_mass *mass;
ENDCOMPONENT(APP_jet_engine)

#ifdef __cplusplus
}
#endif
#endif