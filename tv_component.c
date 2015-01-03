#include "tv_component.h"
#include "transform.h"
#include "tv_entity.h"

tvuint tv_cid_component = 0;

UT_icd tv_component_handler_icd = {
	sizeof(tv_component_handler), NULL, NULL, NULL
};

typedef struct {
	tvuint id;
	tvuint parent_id;
	UT_hash_handle hh;
}id_inheritance_hash_;

typedef struct {
	/* the stage this ID was registered to. */
	tvuint stage;
	/* the offset from that stage's array where the handler function is */
	tvuint stage_offset;
}handler_id_info;

/* the table of arrays representing the ID's that inherit from other ID's.  */
static id_inheritance_hash_ *inheritance_table = NULL;

/* these integers hold the next ID that will be assigned to component types. 
 * TV_COMPONENT_MAX_HANDLERS is reserved for the transform component. */
static tvuint next_free_id = TV_COMPONENT_TRANSFORM_ID + 1;
static tvuint next_free_handler_id = 1;

/* arrays containing all the handlers registered within the engine */
static tv_component_handler registered_handlers[TV_COMPONENT_MAX_STAGES][TV_COMPONENT_MAX_HANDLERS];
static tvuint num_handlers[TV_COMPONENT_MAX_STAGES];

/* an array of arrays corresponding to the components registered to each handler */
static tv_array *registered_components[TV_COMPONENT_MAX_STAGES][TV_COMPONENT_MAX_HANDLERS];

/* information to assist in lookup of handler's ID's. */
static handler_id_info handler_locations[TV_COMPONENT_MAX_HANDLERS];

/* an array of addresses to all the components */
static tv_array *components[TV_COMPONENT_MAX_COMPONENTS];
/* an array to the sizes of all components (and handlers) */
static tvuint component_sizes[TV_COMPONENT_MAX_COMPONENTS];

static void register_inheritance_(tvuint id, tvuint parent_id);

int tv_component_init()
{
	return 0;
}

tv_component *tv_component_copy(tv_component* c)
{
	tv_component *dst = (tv_component*)malloc(component_sizes[c->id]);
	memcpy(dst, c, component_sizes[c->id]);

	/* make the component system aware that this component was added */
	if(c->id > TV_COMPONENT_MAX_HANDLERS) {
		tv_component_notify_add(dst);
	}
	return dst;
}

void tv_component_notify_add(tv_component* c) 
{
	utarray_push_back(components[c->id], &c);
}

void tv_component_free(tv_component *c)
{
	utarray_erase(components[c->id], utarray_eltidx(components[c->id], c), 1);
	free(c);
}

tv_component* tv_component_get(tv_component* self, tvuint id) 
{
	/* if transform is being requested, return the entity's transform. */
	if(id == TV_COMPONENT_MAX_HANDLERS) {
		return (tv_component*)(&(self->entity->transform));
	}
	/* otherwise, get the requested component */
    return tv_entity_get_component(self->entity, id);
}

tvbool tv_component_inherits(tv_component* component, tvuint id)
{
	tvuint i = 0;
	id_inheritance_hash_ *curr;

	i = component->id;

	/* if the ID we're checking for inheritance is the same as the component's, 
	 * we're done. */
	if(i == id) {
		return 1;
	}
	/* keep looking through the inheritance table until the desired ID is either
	 * found, or we've found the root component's (NULL pointer) */
	do {
		HASH_FIND_INT(inheritance_table, &i, curr);
		if(!curr) return 0;
		if(curr->parent_id == id) {
			return 1;
		}
		i = curr->parent_id;
	} while(i != 0);
	return 0;
}

void tv_component_register_id(tvuint *id, tvuint parent_id, tvuint size)
{
	*id = next_free_id;
	register_inheritance_(*id, parent_id);
	utarray_new(components[*id], &ut_ptr_icd);
	component_sizes[*id] = size;
	next_free_id++;
}

void tv_component_register_handler(tvuint *id, tvuint parent_id, void (*func)(tv_component*), tvuint stage, tvuint size)
{
	tv_component_handler handler;
	tv_array *components;

	handler = func;

	utarray_new(components, &ut_ptr_icd);
	utarray_clear(components);
	register_inheritance_(*id, parent_id);
	
	*id = next_free_handler_id++;
	if(next_free_handler_id > TV_COMPONENT_MAX_PRE_HANDLERS) {
		fprintf(stderr, "WARNING: more pre-update handlers than allowed have been registered.\n");
	}

	/* store the handler function */
	registered_handlers[stage][num_handlers[stage]] = handler;
	
	/* initialize an array of components for this handler */
	utarray_new(registered_components[stage][num_handlers[stage]], &ut_ptr_icd);

	/* make it reasonably efficient to lookup this bsns. */
	handler_locations[*id].stage = stage;
	handler_locations[*id].stage_offset = num_handlers[stage];
	component_sizes[*id] = size;

	++num_handlers[stage];
}

void tv_component_register_to_handler(tvuint handler_id, tv_component *component)
{
	handler_id_info id_info = handler_locations[handler_id];
	utarray_push_back(registered_components[id_info.stage][id_info.stage_offset], &component);
}

void tv_component_update_pre_handlers()
{
	tv_component **c;
	tvuint i, j;

	for(i = 0; i < TV_COMPONENT_MAX_PRE_STAGES; ++i) {
		for(j = 0; j < num_handlers[i]; ++j) {
			for(c = (tv_component**)utarray_front(registered_components[i][j]);
				c != NULL;
				c = (tv_component**)utarray_next(registered_components[i][j], c)) {
					registered_handlers[i][j](*c);
			}
		}
	}
}

void tv_component_update_post_handlers()
{
	tv_component **c;
	tvint i, j;
	for(i = TV_COMPONENT_MAX_PRE_STAGES; i < TV_COMPONENT_MAX_POST_STAGES; ++i) {
		for(j = 0; j < num_handlers[i]; ++j) {
			for(c = (tv_component**)utarray_eltidx(registered_components[i][j], j);
				c != NULL;
				c = (tv_component**)utarray_next(registered_components[i][j], c)) {
					registered_handlers[i][j](*c);
			}
		}
	}
}

void register_inheritance_(tvuint id, tvuint parent_id)
{
	id_inheritance_hash_ *s = (id_inheritance_hash_*)tv_alloc(sizeof(id_inheritance_hash_));
	s->id = id;
	s->parent_id = parent_id;
	HASH_ADD_INT(inheritance_table, id, s);
}

tv_array *tv_component_get_all_of_type(tvuint id)
{
	return components[id];
}
