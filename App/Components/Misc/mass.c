#include "mass.h"

void app_mass_add(APP_mass *mass, tvfloat kgs)
{
	mass->kgs += kgs;
}
void app_mass_remove(APP_mass *mass, tvfloat kgs)
{
	mass->kgs -= kgs;
}

COMPONENT_NEW(APP_mass, tv_component)
END_COMPONENT_NEW(APP_mass)

COMPONENT_START(APP_mass)
END_COMPONENT_START

COMPONENT_DESTROY(APP_mass)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(APP_mass)
END_COMPONENT_UPDATE