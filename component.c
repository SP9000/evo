#include "component.h"

TvComponent* tv_component_get(TvComponent* self, int id) 
{
    return tv_entity_get_component(self->entity, id);
}
