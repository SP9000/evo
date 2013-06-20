#include "entity.h"

void Entity_AddComponent(Entity* e, Component* c)
{
    e->children = g_slist_append(e->children, c);
}

void Entity_AddComponents(Entity* e, int numComponents, ...)
{
    int i;
    Component* c;
    va_list vl;

    va_start(vl, numComponents);

    for(i = 1; i < numComponents; ++i) {
        c = va_arg(vl, Component*);
        Entity_AddComponent(e, c);
    }
    va_end(vl);
}

