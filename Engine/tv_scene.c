#include "tv_scene.h"
#include "tv_collision.h"

/*****************************************************************************/
tv_camera *main_cam;	/* a pointer to the main scene camera. */

/*****************************************************************************/
static tvint tv_scene_sort_x(const void* a, const void* b);
static tvint tv_scene_light_sort_x(const void* a, const void* b);

static tv_array *lights = NULL;
static tv_array *entities = NULL;
static tv_scene_sort_func entity_sort = tv_scene_sort_x;
static tv_scene_sort_func light_sort = tv_scene_light_sort_x;

static 	UT_icd light_icd = {sizeof(tv_light), 0,0,0};

/******************************************************************************
 * tv_scene_init
 * Initializes the scene entity array and the scene light array.
******************************************************************************/
tvint tv_scene_init()
{
	utarray_new(lights, &light_icd);
	utarray_new(entities, &ut_ptr_icd);
	return 0;
}
/******************************************************************************
 * tv_scene_to_screen_coordinates 
 * Applies the camera's modelview-projection matrix to the given scene 
 * coordinates and returns the clip coordinates of the given scene position.
******************************************************************************/
tv_vector2 tv_scene_to_screen_coordinates(tv_vector3 scene_coordinates)
{
	tv_vector4 res;
	tv_vector2 ret;
	res.x = scene_coordinates.x;
	res.y = scene_coordinates.y;
	res.z = scene_coordinates.z;
	res.w = 1.0f;
	tv_mat4x4_push(&main_cam->modelview_mat);
	tv_mat4x4_push(&main_cam->projection_mat);

	tv_mat4x4_translate(&main_cam->modelview_mat, main_cam->pos.x, main_cam->pos.y, main_cam->pos.z);
	tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.x, 1.0f, 0.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.y, 0.0f, 1.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.z, 0.0f, 0.0f, 1.0f);
	
	//tv_mat4x4_multiply(main_cam->modelview_mat, main_cam->projection_mat);
	res = tv_mat4x4_multiply_vec4x1(main_cam->modelview_mat, res);
	res = tv_mat4x4_multiply_vec4x1(main_cam->projection_mat, res);
	ret.x = (1.0f / res.w) * res.x * 0.5f + 0.5f;
	ret.y = (1.0f / res.w) * res.y * 0.5f + 0.5f;
	printf("%f %f\n", ret.x, ret.y);
	tv_mat4x4_pop(&main_cam->projection_mat);
	tv_mat4x4_pop(&main_cam->modelview_mat);
	return ret;
}
/******************************************************************************
 * tv_scene_to_screen_coordinates 
 * Applies the inverse of the camera's modelview-projection matrix to turn the
 * given window coordinates to their 3D scene coordinates.
******************************************************************************/
tv_vector3 tv_screen_to_scene_coordinates(tv_vector2 screen_coordinates)
{
	tv_vector3 ret = {0, 0, 0};
	tv_vector4 res = {0, 0, 0, 0};
	TvMat4x4 A;
	TvMat4x4 inv_mvp;

	tv_mat4x4_push(&main_cam->modelview_mat);
	tv_mat4x4_push(&main_cam->projection_mat);

	/* apply matrix transformations */
	tv_mat4x4_load_identity(&main_cam->modelview_mat);
	tv_mat4x4_translate(&main_cam->modelview_mat, main_cam->pos.x, main_cam->pos.y, main_cam->pos.z);
	//tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.x, 1.0f, 0.0f, 0.0f);
	//tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.y, 0.0f, 1.0f, 0.0f);
	//tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.z, 0.0f, 0.0f, 1.0f);
	
	A = tv_mat4x4_multiply(main_cam->modelview_mat, main_cam->projection_mat);
	/* calcualate inverse (and verify if exists) */
	if(!tv_mat4x4_inverse(A, &inv_mvp)) {
		return ret;
	}

	res.x = 2.0f * (screen_coordinates.x) / (tvfloat)screen->w - 1.0f;
	res.y = 2.0f * (screen_coordinates.y - (tvfloat)screen->h) / (tvfloat)screen->h + 1.0f;
	res.z = 1.0f;
	res.w = -1.0f;
	res = tv_mat4x4_multiply_vec4x1(inv_mvp, res);
	if(res.w == 0.0f) {
		return ret;
	}
	res.w = 1.0f / res.w;
	ret.x = res.x * res.w;	//TODO: negative?
	ret.y = res.y * res.w;
	ret.z = res.z * res.w;

	tv_mat4x4_pop(&main_cam->projection_mat);
	tv_mat4x4_pop(&main_cam->modelview_mat);
	return ret;
}
/******************************************************************************
 * tv_scene_quit
 * Frees the scene's entity and light arrays.
******************************************************************************/
tvint tv_scene_quit()
{
	utarray_free(lights);
	utarray_free(entities);
    return 0;
}
/******************************************************************************
 * tv_scene_update
 * does nothing
 * TODO: do something
******************************************************************************/
void tv_scene_update()
{

}
/******************************************************************************
 * tv_scene_add_light
 * Adds a light to the scene's light array.
******************************************************************************/
void tv_scene_add_light(tv_light *light)
{
	utarray_push_back(lights, light); 
	utarray_sort(lights, light_sort);
}
/******************************************************************************
 * tv_scene_lights
 * Returns the scene's light array.
******************************************************************************/
tv_array *tv_scene_lights()
{
	return lights;
}
/******************************************************************************
 * tv_scene_add_entity
 * Adds an entity to the scene's entity array.
******************************************************************************/
void tv_scene_add_entity(tv_entity *entity)
{
	utarray_push_back(entities, entity);
}
/******************************************************************************
 * tv_scene_entities
 * Returns the scene's entity array.
******************************************************************************/
tv_array *tv_scene_entities()
{
	return entities;
}
/******************************************************************************
 * tv_scene_lights_at
 * Returns the light closest to the given position by a brute force search
 * through all the lights in the scene.
******************************************************************************/
tv_array *tv_scene_lights_at(tv_vector3 pos)
{
	tv_light *light;
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
/******************************************************************************
 * tv_scene_set_entity_sort_func
 * TODO: 
******************************************************************************/
void tv_scene_set_entity_sort_func(tv_scene_sort_func func)
{

}
/******************************************************************************
 * tv_scene_set_light_sort_func
 * TODO: 
******************************************************************************/
void tv_scene_set_light_sort_func(tv_scene_sort_func func)
{

}
/******************************************************************************
 * tv_scene_sort_x
 * A simple scene sort function that sorts entities by their X-coordinate.
******************************************************************************/
static tvint tv_scene_sort_x(const void* a, const void* b)
{
	tv_entity *e1 = (tv_entity*)a;
	tv_entity *e2 = (tv_entity*)b;
	return (tvint)(e1->transform.pos.x - e1->transform.pos.x);
}
/******************************************************************************
 * tv_scene_light_sort_x
 * A simple scene sort function that sorts lights by their X-coordinate.
******************************************************************************/
static tvint tv_scene_light_sort_x(const void* a, const void* b)
{
	tv_component *l1 = (tv_component*)a;
	tv_component *l2 = (tv_component*)b;

	return (tvint)(l1->entity->transform.pos.x - l2->entity->transform.pos.x);
}
/******************************************************************************
 * tv_scene_raypick
 * TODO: is this the raycast function I use?
******************************************************************************/
tv_array* tv_scene_raypick(tv_vector2 screen_coordinates)
{
	tv_vector3 start, dir;
	tv_vector4 v;
	TvMat4x4 pv_mat;
	TvMat4x4 inv_vp;

	tv_vector2 clip_coords;
	tv_vector4 world1, world2;

	clip_coords.x = (screen_coordinates.x) / (tvfloat)screen->w * 2.0f - 1.0f;
	clip_coords.y = ((tvfloat)screen->h - screen_coordinates.y) / (tvfloat)screen->h * 2.0f - 1.0f;

	/* preserve matrices */
	tv_mat4x4_push(&main_cam->modelview_mat);
	tv_mat4x4_push(&main_cam->projection_mat);

	/* apply view transformations to get view matrix (no model transformations) */
	tv_mat4x4_load_identity(&main_cam->modelview_mat);
	tv_mat4x4_translate(&main_cam->modelview_mat, main_cam->pos.x, main_cam->pos.y, main_cam->pos.z);
	//tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.x, 1.0f, 0.0f, 0.0f);
	//tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.y, 0.0f, 1.0f, 0.0f);
	//tv_mat4x4_rotate(&main_cam->modelview_mat,  main_cam->rot.z, 0.0f, 0.0f, 1.0f);
	
	/* perspective-view matrix */
	pv_mat = tv_mat4x4_multiply(main_cam->projection_mat, main_cam->modelview_mat);
	/* calcualate inverse (and verify if exists) */
	if(!tv_mat4x4_inverse(pv_mat, &inv_vp)) {
		return NULL;
	}
	/* near plane */
	v.x = clip_coords.x;
	v.y = clip_coords.y;
	v.z = -1.0f;
	v.w = 1.0f;
	world1 = tv_mat4x4_multiply_vec4x1(inv_vp, v);
	if(world1.w == 0.0f) {
		return NULL;
	}
	world1.w = 1.0f / world1.w;
	world1.x *= world1.w;
	world1.y *= world1.w;
	world1.z *= world1.w;

	/* far plane */
	v.z = 0.0f;
	world2 = tv_mat4x4_multiply_vec4x1(inv_vp, v);
	if(world2.w == 0.0f) {
		return NULL;
	}
	world2.w = 1.0f / world2.w;
	world2.x *= world2.w;
	world2.y *= world2.w;
	world2.z *= world2.w;

	/* get the direction */
	tv_vector4_sub(world2, world1, &v);
	dir.x = v.x;
	dir.y = v.y;
	dir.z = v.z;
	tv_vector3_normalize(dir, &dir);

	/* restore matrices */
	tv_mat4x4_pop(&main_cam->projection_mat);
	tv_mat4x4_pop(&main_cam->modelview_mat);


	start.x = world1.x;
	start.y = world1.y;
	start.z = world1.z;
	printf("casting ray: origin @ (%f, %f, %f)\n  direction: (%f, %f, %f)\n", start.x, start.y, start.z, dir.x, dir.y, dir.z);
	return tv_scene_raycast(start, dir, TV_INF);
}
/******************************************************************************
 * tv_scene_raycast
 * TODO:
******************************************************************************/
tv_array* tv_scene_raycast(tv_vector3 start, tv_vector3 dir, tvfloat len)
{
	TV_collider ray;

	/* create the line collider and check collision */
	tv_collider_line(&ray, start, dir, TV_INF);
	return tv_collision_check(&ray);
}