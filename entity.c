#include "entity.h"

static TvEntityList* entities;

TvEntity* tv_entity_new()
{
    TvEntity* e = (TvEntity*)malloc(sizeof(TvEntity)); 
    e->components = NULL; 
    e->children = NULL; 
    return e;
}

void tv_entity_add_component(TvEntity* e, TvComponent* c)
{
	TvComponentList* node = (TvComponentList*)tv_alloc(sizeof(TvComponentList));
	LL_APPEND(e->components, node);
    c->entity = e;
}

void tv_entity_add_child(TvEntity* parent, TvEntity* child)
{
	TvEntityList* node = (TvEntityList*)tv_alloc(sizeof(TvEntityList));
	LL_APPEND(parent->children, node);
	child->parent = parent;
}

void tv_entity_start(TvEntity* e)
{
    TvComponentList* clist;
	TvEntityList* node;
    for(clist = e->components; clist != NULL; clist = clist->next) {
        /* get the component and start it */
        TvComponent* c = (TvComponent*)clist;
        c->Start(c);
    }
	node = (TvEntityList*)tv_alloc(sizeof(TvEntityList));
	LL_APPEND(entities, node); 
}

TvComponent* tv_entity_get_component(TvEntity* e, int cid)
{
    TvComponentList* clist;
    if(!e) {
        return NULL;
    }
    for(clist = e->components; clist != NULL; clist = clist->next) {
        TvComponent* c = (TvComponent*)clist;
        if(c->id == cid) {
            return c;
        }   
    }
    return NULL;
}

void tv_entity_update()
{
    TvEntityList* eit;
    TvComponentList* cit;

    /* foreach entity... */
    for(eit = entities; eit != NULL; eit = eit->next) {
        /* update all components for this entity */
		TvEntity* e = (TvEntity*)(eit->e);
        for(cit = e->components; cit != NULL; cit = cit->next) {
			TvComponent* c = (TvComponent*)cit;
			c->Update(c);
        }
    }
}
