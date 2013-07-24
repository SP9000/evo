#include "entity.h"

static GSList* entities;

Entity* Entity_New()
{
    Entity* e = (Entity*)malloc(sizeof(Entity)); 
    e->numChildren = 0; 
    e->numComponents = 0; 
    e->components = NULL; 
    e->children = NULL; 
    return e;
}

void Entity_AddComponent(Entity* e, Component* c)
{
    e->components = g_slist_append(e->components, c);
    c->entity = e;
}

void Entity_AddChild(Entity* parent, Entity* child)
{
    parent->children = g_slist_append(parent->children, child);
    child->parent = parent;
}

void Entity_Start(Entity* e)
{
    GSList* clist;
    for(clist = e->components; clist != NULL; clist = g_slist_next(clist)) {
        /* get the component and start it */
        Component* c = (Component*)clist->data;
        c->Start(c);

        /* add certain types of components to parts of the engine */
        switch(c->id) {
        case CID_Collider:
            Collision_AddCollider((Component_Collider*)c);
            break;
        case CID_Renderer:
        case CID_ModelRenderer:
        case CID_TextRenderer:
            Scene_Add((Component_Renderer*)c);
            break;
        case CID_Widget:
            //Scene_AddWidget((Component_Widget*)c);
            break;
        case CID_Camera:
            /* If no camera has been created, set this one as the main cam. */
            if(!main_cam) {
                Draw_AddCamera((Component_Camera*)c);
            }
            break;
        default:
            break;
        }
    }
    entities = g_slist_append(entities, e);
}

Component* Entity_GetComponent(Entity* e, int cid)
{
    GSList* clist;
    if(!e) {
        return NULL;
    }
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
        c->Collide(other);
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
            ((Component*)(cit->data))->Update(cit->data);
        }
    }
}
