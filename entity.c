#include "entity.h"

static GSList* entities;

TvEntity* Entity_New()
{
    TvEntity* e = (TvEntity*)malloc(sizeof(TvEntity)); 
    e->numChildren = 0; 
    e->numComponents = 0; 
    e->components = NULL; 
    e->children = NULL; 
    return e;
}

void tv_entity_add_component(TvEntity* e, Component* c)
{
    e->components = g_slist_append(e->components, c);
    c->entity = e;
}

void tv_entity_add_child(TvEntity* parent, TvEntity* child)
{
    parent->children = g_slist_append(parent->children, child);
    child->parent = parent;
}

void tv_entity_start(TvEntity* e)
{
    GSList* clist;
    for(clist = e->components; clist != NULL; clist = g_slist_next(clist)) {
        /* get the component and start it */
        Component* c = (Component*)clist->data;
        c->Start(c);

        /* add certain types of components to parts of the engine */
        switch(c->id) {
        case CID_Collider:
			tv_collision_add_collider((Component_Collider*)c);
            break;
        case CID_Renderer:
        case CID_ModelRenderer:
        case CID_TextRenderer:
            tv_scene_add((Component_Renderer*)c);
            break;
        case CID_Widget:
            //Scene_AddWidget((Component_Widget*)c);
            break;
        case CID_Camera:
            /* If no camera has been created, set this one as the main cam. */
            if(!main_cam) {
                tv_draw_add_camera((Component_Camera*)c);
            }
            break;
        default:
            break;
        }
    }
    entities = g_slist_append(entities, e);
}

Component* Entity_GetComponent(TvEntity* e, int cid)
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

void Entity_Collide(TvEntity* e, TvEntity* other)
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
        for(cit = ((TvEntity*)(eit->data))->components; cit != NULL; cit = g_slist_next(cit)) {
            ((Component*)(cit->data))->Update(cit->data);
        }
    }
}
