#include "component.h"

Component* Component_Get(Component* self, int id) 
{
    return Entity_GetComponent(self->entity, id);
}
