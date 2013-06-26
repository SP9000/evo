#include "entity.h"

void Entity_AddComponent(Entity* e, Component* c)
{
    e->components = g_slist_append(e->components, c);
    c->entity = e;
}

void Entity_AddComponents(Entity* e, int numComponents, ...)
{
    int i;
    Component* c;
    va_list vl;

    va_start(vl, numComponents);
    for(i = 0; i < numComponents; ++i) {
        c = va_arg(vl, Component*);
        Entity_AddComponent(e, c);
    }
    va_end(vl);
}

void Entity_Start(Entity* e)
{
    GSList* clist;
    for(clist = e->components; clist != NULL; clist = g_slist_next(clist)) {
        /* get the component and start it */
        Component* c = (Component*)clist->data;
        c->start(c);

        /* add certain types of components to parts of the engine */
        switch(c->id) {
        case CID_Collider:
            Collision_AddCollider((Component_Collider*)c);
            break;
        default:
            break;
        }
    }
}

Component* Entity_GetComponent(Entity* e, int cid)
{
    GSList* clist;
    for(clist = e->components; clist != NULL; clist = g_slist_next(clist)) {
        Component* c = (Component*)clist->data;
        if(c->id == cid) {
            return c;
        }   
    }
    return NULL;
}

void Entity_Collide(Entity* e, Entity* other)
{
    GSList* clist;
    for(clist = e->components; clist != NULL; clist = g_slist_next(clist)) {
        Component* c = (Component*)clist->data;
        c->collide(other);
    }
    
}
