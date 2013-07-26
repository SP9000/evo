#include "entity.h"

static GSList* entities;

TvEntity* tv_entity_new()
{
    TvEntity* e = (TvEntity*)malloc(sizeof(TvEntity)); 
    e->numChildren = 0; 
    e->numComponents = 0; 
    e->components = NULL; 
    e->children = NULL; 
    return e;
}

void tv_entity_add_component(TvEntity* e, TvComponent* c)
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
        TvComponent* c = (TvComponent*)clist->data;
        c->Start(c);
    }
    entities = g_slist_append(entities, e);
}

TvComponent* tv_entity_get_component(TvEntity* e, int cid)
{
    GSList* clist;
    if(!e) {
        return NULL;
    }
    for(clist = e->components; clist != NULL; clist = g_slist_next(clist)) {
        TvComponent* c = (TvComponent*)clist->data;
        if(c->id == cid) {
            return c;
        }   
    }
    return NULL;
}

void tv_entity_update()
{
    GSList* eit;
    GSList* cit;

    /* foreach entity... */
    for(eit = entities; eit != NULL; eit = g_slist_next(eit)) {
        /* update all components for this entity */
		TvEntity* e = (TvEntity*)eit->data;
        for(cit = e->components; cit != NULL; cit = g_slist_next(cit)) {
			TvComponent* c = (TvComponent*)cit->data;
			c->Update(c);
        }
    }
}
