#include "entity.h"

static TvArray /*Entity* */ *entities;

int tv_entity_init()
{
	utarray_new(entities, &ut_ptr_icd);
	return 0;
}

TvEntity* tv_entity_new()
{
    TvEntity* e = (TvEntity*)malloc(sizeof(TvEntity)); 
    e->numChildren = 0; 
    e->numComponents = 0; 
	e->transform.pos.x = 0;
	e->transform.pos.y = 0;
	e->transform.pos.z = 0;
	e->transform.scale.x = 1.0f;
	e->transform.scale.y = 1.0f;
	e->transform.scale.z = 1.0f;
	e->transform.rot.x = 0.0f;
	e->transform.rot.y = 0.0f;
	e->transform.rot.z = 0.0f;
	utarray_new(e->components, &ut_ptr_icd);
	utarray_new(e->children, &ut_ptr_icd);
    return e;
}

void tv_entity_add_component(TvEntity* e, tv_component* c)
{
	c->entity = e;
	utarray_push_back(e->components, &c);
}

void tv_entity_add_child(TvEntity* parent, TvEntity* child)
{
	child->parent = parent;
	utarray_push_back(parent->children, &child);
}

void tv_entity_start(TvEntity* e)
{
	tv_component **c;
	/* foreach component... */
	for(c = (tv_component**)utarray_front(e->components); 
		c != NULL; 
		c = (tv_component**)utarray_next(e->components, c)) {
        /* start the component */
        (*c)->Start(*c);
    }
	/* add the entity to the internal array of entities. */
	utarray_push_back(entities, &e);
}

tv_component* tv_entity_get_component(TvEntity* e, tvuint cid)
{
	tv_component **c;
	if(e == NULL) {
        return NULL;
    }
	/* iterate through all the components of the given entity looking for a 
	 * component of the desired ID. */
	for(c = (tv_component**)utarray_front(e->components); 
		c != NULL; 
		c = (tv_component**)utarray_next(e->components, c)) {
        if((*c)->id == cid) {
            return *c;
        }   
    }
	/* a component of the desired ID does not exist within the given entity. */
    return NULL;
}

void tv_entity_update()
{
	TvEntity **e;
	TvEntity **child;
	tv_component **c;
	
	/* Run the update component of each component in every entity (including 
	 * those that are children of other entitites. */
    /* foreach entity... */
    for(e = (TvEntity**)utarray_front(entities); 
		e != NULL;
		e = (TvEntity**)utarray_next(entities, e)) 
	{
		/* update all components for this entity */
		for(c = (tv_component**)utarray_front((*e)->components); 
			c != NULL;
			c = (tv_component**)utarray_next((*e)->components, c))
		{
			(*c)->Update(*c);
		}
		/* foreach child of the entity... */
		for(child = (TvEntity**)utarray_front((*e)->children);
			child != NULL;
			child = (TvEntity**)utarray_next((*e)->children, child)) 
		{
				/* update all components for this entity */
				for(c = (tv_component**)utarray_front((*e)->components); 
					c != NULL;
					c = (tv_component**)utarray_next((*e)->components, c))
				{
					(*c)->Update(*c);
				}
		}
	}
}
