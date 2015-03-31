#include "jet_engine.h"

COMPONENT_NEW(APP_jet_engine, tv_component)
	self->kg = 100.0f;
	self->max_power = 100.0f;
END_COMPONENT_NEW(APP_jet_engine)

COMPONENT_START(APP_jet_engine)
	self->mass = (APP_mass*)tv_component_get(self_component, APP_mass_id());
END_COMPONENT_START

COMPONENT_UPDATE(APP_jet_engine)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(APP_jet_engine)
END_COMPONENT_DESTROY