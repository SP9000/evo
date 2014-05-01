#include "scene.h"

static tvint tv_scene_sort_x(const void* a, const void* b);
static tvint tv_scene_light_sort_x(const void* a, const void* b);

static tv_array *lights = NULL;
static tv_array *entities = NULL;
static tv_scene_sort_func entity_sort = tv_scene_sort_x;
static tv_scene_sort_func light_sort = tv_scene_light_sort_x;

static 	UT_icd light_icd = {sizeof(tv_light), 0,0,0};

tv_camera *main_cam;

tvint tv_scene_init()
{
	utarray_new(lights, &light_icd);
	utarray_new(entities, &ut_ptr_icd);
	return 0;
}

tvint tv_scene_quit()
{
    return 0;
}

void tv_scene_update()
{

}

void tv_scene_add_light(tv_light *light)
{
	utarray_push_back(lights, light); 
	utarray_sort(lights, light_sort);
}
tv_array *tv_scene_lights()
{
	return lights;
}

void tv_scene_add_entity(tv_entity *entity)
{
	utarray_push_back(entities, entity);
}
tv_array *tv_scene_entities()
{
	return entities;
}

tv_array *tv_scene_lights_at(tv_vector3 pos)
{
	tv_light *light;
	tv_light *closest;
	tv_array *ret;

	utarray_new(ret, &light_icd);
	/* find the light closest to the given position */
	for(light = (tv_light*)utarray_front(lights);
		light != NULL;
		light = (tv_light*)utarray_next(lights, light)) {
			if(tv_vector3_distance(((tv_component*)light)->entity->transform.pos, pos) < light->range) {
				utarray_push_back(ret, light);
			}
	}
	return ret;
}

void tv_scene_set_entity_sort_func(tv_scene_sort_func func)
{

}

void tv_scene_set_light_sort_func(tv_scene_sort_func func)
{

}

static tvint tv_scene_sort_x(const void* a, const void* b)
{
	tv_entity *e1 = (tv_entity*)a;
	tv_entity *e2 = (tv_entity*)b;
	return (tvint)(e1->transform.pos.x - e1->transform.pos.x);
}
static tvint tv_scene_light_sort_x(const void* a, const void* b)
{
	tv_component *l1 = (tv_component*)a;
	tv_component *l2 = (tv_component*)b;

	return (tvint)(l1->entity->transform.pos.x - l2->entity->transform.pos.x);
}
