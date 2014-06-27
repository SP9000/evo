#include "entity.h"

static tv_array /*Entity* */ *entities;

int tv_entity_init()
{
	utarray_new(entities, &ut_ptr_icd);
	return 0;
}

tv_entity* tv_entity_new(tv_transform *transform)
{
    tv_entity* e = (tv_entity*)malloc(sizeof(tv_entity)); 
    e->numChildren = 0; 
    e->numComponents = 0; 
	if(transform == NULL) {
		e->transform.pos.x = 0;
		e->transform.pos.y = 0;
		e->transform.pos.z = 0;
		e->transform.scale.x = 1.0f;
		e->transform.scale.y = 1.0f;
		e->transform.scale.z = 1.0f;
		e->transform.rot.x = 0.0f;
		e->transform.rot.y = 0.0f;
		e->transform.rot.z = 0.0f;
	}
	else {
		e->transform.pos.x = transform->pos.x;
		e->transform.pos.y = transform->pos.y;
		e->transform.pos.z = transform->pos.z;
		e->transform.scale.x = transform->scale.x;
		e->transform.scale.y = transform->scale.y;
		e->transform.scale.z = transform->scale.z;
		e->transform.rot.x = transform->rot.x;
		e->transform.rot.y = transform->rot.y;
		e->transform.rot.z = transform->rot.z;
	}
	utarray_new(e->components, &ut_ptr_icd);
	utarray_new(e->children, &ut_ptr_icd);
    return e;
}

tv_component*  tv_entity_add_component(tv_entity* e, tv_component* c)
{
	c->entity = e;
	c->transform = &(e->transform);
	utarray_push_back(e->components, &c);
	/* TODO: check that no component of c's type exists. */
	return c;
}

void tv_entity_add_child(tv_entity* parent, tv_entity* child)
{
	child->parent = parent;
	utarray_push_back(parent->children, &child);
}

void tv_entity_start(tv_entity* e)
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

tv_component* tv_entity_get_component(tv_entity* e, tvuint cid)
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
		if(tv_component_inherits(*c, cid)) {
            return *c;
        }   
    }
	/* a component of the desired ID does not exist within the given entity. */
    return NULL;
}

void tv_entity_send_message(tv_entity *sender, tv_entity *receiver, tv_message_type message_type, tv_message message)
{

}

void tv_entity_receive_message(tv_entity *sender, tv_entity *receiver, tv_message_type message_type, tv_message message)
{
	tv_entity **child;
	tv_component **c;
	/* foreach child... */
	for(child = (tv_entity**)utarray_front(receiver->children);
		child != NULL;
		child = (tv_entity**)utarray_next(receiver->children, child)) 
	{
			/* update all components for this entity */
			for(c = (tv_component**)utarray_front((*child)->components); 
				c != NULL;
				c = (tv_component**)utarray_next((*child)->components, c))
			{
				tv_component_receive_message(sender, message_type, message);
			}
	}
}

void tv_entity_update()
{
	tv_entity **e;
	tv_entity **child;
	tv_component **c;
	
	/* Run the update component of each component in every entity (including 
	 * those that are children of other entitites. */
    /* foreach entity... */
    for(e = (tv_entity**)utarray_front(entities); 
		e != NULL;
		e = (tv_entity**)utarray_next(entities, e)) 
	{
		/* update all components for this entity */
		for(c = (tv_component**)utarray_front((*e)->components); 
			c != NULL;
			c = (tv_component**)utarray_next((*e)->components, c))
		{
			(*c)->Update(*c);
		}
		/* foreach child of the entity... */
		for(child = (tv_entity**)utarray_front((*e)->children);
			child != NULL;
			child = (tv_entity**)utarray_next((*e)->children, child)) 
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
