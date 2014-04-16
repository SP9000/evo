#include "component.h"

tvuint tv_cid_component = 0;

UT_icd tv_component_handler_icd = {
	sizeof(tv_component_handler), NULL, NULL, NULL
};

typedef struct {
	tvuint id;
	tvuint parent_id;
	UT_hash_handle hh;
}id_inheritance_hash_;

/* the table of arrays representing the ID's that inherit from other ID's.  */
static id_inheritance_hash_ *inheritance_table = NULL;

/* these integers hold the next ID that will be assigned to component types. */
static tvuint next_free_id = TV_COMPONENT_MAX_HANDLERS;
static tvuint next_free_pre_handler_id = 1;
static tvuint next_free_post_handler_id = TV_COMPONENT_MAX_PRE_HANDLERS;

/* arrays containing all the handlers registered within the engine */
static TvArray *registered_pre_handlers;
static TvArray *registered_post_handlers;

/* an array of arrays corresponding to the components registered to each handler */
static TvArray *registered_pre_components;
static TvArray *registered_post_components;

static void register_inheritance_(tvuint id, tvuint parent_id);

static void *dummy_ = 0;

int tv_component_init()
{
	utarray_new(registered_pre_handlers, &ut_ptr_icd);
	utarray_new(registered_post_handlers, &ut_ptr_icd);
	utarray_new(registered_pre_components, &ut_ptr_icd);
	utarray_new(registered_post_components, &ut_ptr_icd);

	/* set the first element of the handlers to NULL (no NULL ID's) */
	utarray_clear(registered_pre_handlers);
	utarray_clear(registered_pre_components);
	utarray_push_back(registered_pre_handlers, &dummy_);
	utarray_push_back(registered_pre_components, &dummy_);

	return 0;
}

tv_component* tv_component_get(tv_component* self, tvuint id) 
{
    return tv_entity_get_component(self->entity, id);
}

tvbool tv_component_inherits(tv_component* component, tvuint id)
{
	tvuint i = 0;
	id_inheritance_hash_ *curr;

	i = component->id;
	do {
		HASH_FIND_INT(inheritance_table, &i, curr);
		if(curr->id == id) {
			return 1;
		}
		i = curr->parent_id;
	} while(i != 0);
	return 0;
}

void tv_component_register_id(tvuint *id, tvuint parent_id)
{
	*id = next_free_id++;
	register_inheritance_(*id, parent_id);
}

void tv_component_register_handler(tvuint *id, tvuint parent_id, void (*func)(tv_component*), tvbool pre_update)
{
	tv_component_handler handler;
	TvArray *components;

	handler = func;

	utarray_new(components, &ut_ptr_icd);
	utarray_clear(components);
	register_inheritance_(*id, parent_id);
	if(pre_update) {
		*id = next_free_pre_handler_id++;
		if(next_free_pre_handler_id > TV_COMPONENT_MAX_PRE_HANDLERS) {
			fprintf(stderr, "WARNING: more pre-update handlers than allowed have been registered.\n");
		}
		utarray_push_back(registered_pre_handlers, &handler);
		utarray_push_back(registered_pre_components, &components);
	}
	else {
		*id = next_free_post_handler_id++;
		if(next_free_post_handler_id > TV_COMPONENT_MAX_POST_HANDLERS) {
			fprintf(stderr, "WARNING: more post-update handlers than allowed have been registered.\n");
		}
		utarray_push_back(registered_post_handlers, &handler);
		utarray_push_back(registered_post_components, &components);
	}
}

void tv_component_register_to_handler(tvuint handler_id, tv_component *component, tvbool pre_update)
{
	if(pre_update) {
		utarray_push_back((*(TvArray**)utarray_eltptr(registered_pre_components, handler_id)), &component);
		puts("registered");
	}
	else {
		utarray_push_back((TvArray*)utarray_eltptr(registered_post_components, handler_id), &component);
	}
}

void tv_component_update_pre_handlers()
{
	tv_component_handler *handler;
	TvArray **components;
	tv_component **c;
	int index;

	for(handler = (tv_component_handler*)utarray_eltptr(registered_pre_handlers, 1), index = 1;
		handler != NULL;
		handler = (tv_component_handler*)utarray_next(registered_pre_handlers, handler), ++index) {
			components = (TvArray**)utarray_eltptr(registered_pre_components, index);
			for(c = (tv_component**)utarray_front(*components);
				c != NULL;
				c = (tv_component**)utarray_next(*components, c)) {
					(*handler)(*c);
			}
	}
}

void tv_component_update_post_handlers()
{

}

void register_inheritance_(tvuint id, tvuint parent_id)
{
	id_inheritance_hash_ *s = (id_inheritance_hash_*)tv_alloc(sizeof(id_inheritance_hash_));
	s->id = id;
	s->parent_id = id;
	HASH_ADD_INT(inheritance_table, id, s);
}