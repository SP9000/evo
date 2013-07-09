#include "entity.h"

static GSList* entities;

Entity* Entity_New(int numComponents, ...)
{
    int i;
    Component* c;
    va_list vl;

    int hasTransform = 0;

    Entity* e = (Entity*)malloc(sizeof(Entity)); 
    e->numChildren = 0; 
    e->numComponents = 0; 
    e->components = NULL; 
    e->children = NULL; 

    va_start(vl, numComponents);
    for(i = 0; i < numComponents; ++i) {
        c = va_arg(vl, Component*);
        if(c->id == CID_Transform) {
            hasTransform = 1;
        }
        Entity_AddComponent(e, c);
    }
    va_end(vl);

    /* if no transform was given, add a default one */
    if(!hasTransform) {
        Vector3 dfltPos = {0.0f, 0.0f, 0.0f};
        Vector3 dfltScale = {1.0f, 1.0f, 1.0f};
        Entity_AddComponent(e, Component_New_Transform(NULL, dfltPos, dfltScale));
    }

    Entity_Start(e); 
    entities = g_slist_append(entities, e);
    return e;
}

void Entity_AddComponent(Entity* e, Component* c)
{
    e->components = g_slist_append(e->components, c);
    c->entity = e;
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
        case CID_Model:
            Scene_Add((Component_Model*)c);
            break;
        case CID_Widget:
            Scene_AddWidget((Component_Widget*)c);
            break;
        case CID_Camera:
            /* If no camera has been created, set this one as the main cam. */
            if(!main_cam) {
                main_cam = (Component_Camera*)c;
                Draw_SetCamera(main_cam);
            }
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

void Entity_Update()
{
    GSList* eit;
    GSList* cit;

    /* foreach entity... */
    for(eit = entities; eit != NULL; eit = g_slist_next(eit)) {
        /* update all components for this entity */
        for(cit = ((Entity*)(eit->data))->components; cit != NULL; cit = g_slist_next(cit)) {
            ((Component*)(cit->data))->update(cit->data);
        }
    }
}
