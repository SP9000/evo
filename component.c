#include "component.h"

Component* Component_Get(Component* self, int id) 
{
    if(self->entity != NULL) {
        return Entity_GetComponent(self->entity, id);
    }
    return NULL;
}
