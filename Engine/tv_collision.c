#include "tv_collision.h"


/******************************************************************************
/* TODO: implement constructor/destructor */
UT_icd  tv_collider_icd = {sizeof(TV_collider), 0, 0, 0};

/******************************************************************************
 * struct DetectCollisionHash
 * This structure is used to store the collision detection function associated
 * with a component ID.  The component ID is used as the key to look up the
 * function. When an entity with a component known by that ID collides with 
 * another collider, the function is called on that component.
 * e.g.:
 *  For the entity with the following components:
 *   e = entity(transform, box_collider, apple) 
 * The apple_collide function is registered as shown below.
 *   tv_collision_register_component(apple_collide, apple_id())
 * Now, when e's box_collider collides with another entity, apple_collide is
 * called as shown below:
 *   apple_collide(apple)
 *****************************************************************************/
typedef struct DetectCollisionHash {
	tvuint cid;
	TvCollisionDetectFunc f;
	TvHashHandle hh;
}DetectCollisionHash;

/*****************************************************************************/
/* table of registered collider ID's and their OnCollide functions */
static DetectCollisionHash* registered_colliders; 
static DetectCollisionHash* registered_callbacks;
/* lists of all the colldiers sorted on various axes for fast detection */
static tv_array *colliders;
static tv_array *xSorted;

/******************************************************************************
 * tv_entity_collide (proto)
 * A function to propogate collisions from a collider to all components that 
 * have a registered collide function attached to the colliding entity. 
 *****************************************************************************/
void tv_entity_collide(tv_entity* e, tv_entity* other);


/******************************************************************************
 * tv_collision_init
 * Initializes the necessary data for other collision detection functions to 
 * work properly.  This includes the collider and callback tables.
 *****************************************************************************/
tvint tv_collision_init()
{
    utarray_new(colliders, &ut_ptr_icd);
	registered_colliders = NULL;
	registered_callbacks = NULL;
	return 0;
}
/******************************************************************************
 * tv_collision_register_collider
 * Appends the collider to the internal array of colliders so that it may be 
 * checked for collision against other colliders.
 *****************************************************************************/
void tv_collision_register_collider(TV_collider* collider)
{
	utarray_push_back(colliders, &collider);
	/* TODO: resort */
}
/******************************************************************************
 * tv_collision_register_component
 * Adds a collision function to the "on collide" table.  When a collision with
 * the component known by the given ID occurs, this function is looked up
 * in this table and called.
 *****************************************************************************/
void tv_collision_register_component(TvCollisionCollideFunc on_collision, tvuint id)
{
	DetectCollisionHash* entry = (DetectCollisionHash*)tv_alloc(sizeof(DetectCollisionHash));
	entry->cid = id;
	entry->f = on_collision;
	HASH_ADD_INT(registered_callbacks, cid, entry);
}
/******************************************************************************
 * tv_collision_add_entity
 * TODO:
 *****************************************************************************/
void tv_collision_add_entity(tv_entity* e)
{
	utarray_push_back(colliders, e);
	/* TODO //colliding = g_hash_table_new(NULL, NULL); */
}
/******************************************************************************
 * tv_collision_remove_entity
 * TODO:
 *****************************************************************************/
void tv_collision_remove_entity(tv_entity* e)
{
	utarray_erase(colliders,
		utarray_eltidx(colliders, e),
		1);
}
/******************************************************************************
 * tv_collision_detect
 * The detect function is called every frame to check if any collisions 
 * occured between any of the registered colliders.
 *****************************************************************************/
void tv_collision_detect()
{
	tv_entity* c1;
    tv_entity* c2;
	UT_array* possibleCollisions;
 
	utarray_new(possibleCollisions, &ut_ptr_icd);

    /* Check X */
    for(c1 = (tv_entity*)xSorted, c2 = (tv_entity*)xSorted+1; 
		c1 != NULL;
		c2 = (tv_entity*)utarray_next(xSorted,c2)) {

        /* end of the array? */
        if(c2 == NULL) {
            c2 = c1 = (tv_entity*)utarray_next(xSorted, c1);
        }
        else {
            /* not overlapping? */
            if((c1->transform.pos.x + c1->aabb.w) <
                     (c2->transform.pos.x)) {
                c2 = c1 = (tv_entity*)utarray_next(xSorted, c1);
            }
            /* X is overlapping */
            else {
				if(c1->transform.pos.y > c2->transform.pos.y) {
					c1 = c2; 
					c2 = c1;
				}
				if(c1->aabb.w + c1->transform.pos.y >= c2->transform.pos.y) {
					/* Y is overlapping */
					if(c1->transform.pos.z > c2->transform.pos.z) {
						c1 = c2; 
						c2 = c1;
					}
					if(c1->aabb.d + c1->transform.pos.z >= c2->transform.pos.z) {
						/* collison */
						tv_entity_collide(c1, c2);
						fflush(stdout);
					}
				}
			}
        }
    } 
    /* colliding now contains all pairs of colliders who's AABB's are 
     * overlapping...do more precise detection TODO */
	/* clean up */
	utarray_free(possibleCollisions);
}
/******************************************************************************
 * tv_entity_collide
 * TODO:
 *****************************************************************************/
void tv_entity_collide(tv_entity* e, tv_entity* other)
{
	DetectCollisionHash* on_collide;
	DetectCollisionHash* detect;
	/* do precise collision detection */
	HASH_FIND_INT(registered_colliders, &e->collide, detect);
	if(detect) {
		/* call e's collide function */
		HASH_FIND_INT(registered_colliders, &e->collide, on_collide);
		if(on_collide) {
			on_collide->f(other);
		}
		/* call other's collide function */
		HASH_FIND_INT(registered_colliders, &other->collide, on_collide);
		if(on_collide) {
			on_collide->f(e);
		}
	}
}
/******************************************************************************
 * tv_entity_update
 * TODO: sort the collider arrays (could also be done per-frame in detect)
 *****************************************************************************/
void tv_collision_update()
{

}
/******************************************************************************
 * tv_physics_check_ray_triangle
 * TODO:
 *****************************************************************************/
tv_vector3 tv_physics_check_ray_triangle(tv_vector3 p1, tv_vector3 p2, TV_collider *triangle)
{
}
/******************************************************************************
 * tv_physics_check_ray_plane
 * TODO:
 *****************************************************************************/
tv_vector3 tv_physics_check_ray_plane(tv_vector3 p1, tv_vector3 p2, TV_collider *plane)
{
	tvfloat w = plane->info.quad.w;
	tvfloat h = plane->info.quad.h;

	/* temporary triangle collider */
	TV_collider t0;

	/* TODO: apply transformations (rot/scale/trans) to the quad */
	tv_vector3 v0 = tv_vector3_new(0.0f, 0.0f, 0.0f);
	tv_vector3 v1 = tv_vector3_new(w, 0.0f, 0.0f);
	tv_vector3 v2 = tv_vector3_new(w, h, 0.0f);
	tv_vector3 v3 = tv_vector3_new(0.0f, h, 0.0f);

	/* decompose plane into triangles and test those for intersection */
	/* TODO: obviously this is far from the best way */
	t0 = *plane;
	t0.type = TV_COLLIDER_TRIANGLE;
	t0.info.triangle.v0 = tv_vector3_new(0.0f, 0.0f, 0.0f);
	t0.info.triangle.v1 = tv_vector3_new(w, 0.0f, 0.0f);
	t0.info.triangle.v2 = tv_vector3_new(w, h, 0.0f);

	return tv_physics_check_ray_triangle(p1, p2, &t0);
	//TODO: check other half */
	//t0.info.triangle.v1 = tv_vector3_new(w, h, 0.0f);
	//t0.info.triangle.v2 = tv_vector3_new(w, h, 0.0f);
	//return tv_physics_check_ray_triangle(p1, p2, &t0)
}
/******************************************************************************
 * tv_physics_check_ray_sphere
 * TODO:
 *****************************************************************************/
tv_vector3 tv_physics_check_ray_sphere(tv_vector3 p1, tv_vector3 p2, TV_collider *sphere)
{
	/* heavily inspired by:
	* atelier iebele abel - 2001
	* http://paulbourke.net/geometry/circlesphere/source.cpp 
	*/
	tv_vector3 pt;
	tv_vector3 sp = sphere->pos;
	tvfloat r = sphere->info.sphere.radius;
	tv_vector3 a;
	tvfloat i;

	a.x = tv_sqr(p2.x - p1.x) + tv_sqr(p2.y - p1.y) + tv_sqr(p2.z - p1.z);
	a.y = 2 * ((p2.x - p1.x)*(p1.x - sp.x) +
		(p2.y - p1.y) * (p1.y - sp.y) +
		(p2.z - p1.y) * (p1.z - sp.z));
	a.z = tv_sqr(sp.x) + tv_sqr(sp.y) + 
		tv_sqr(sp.z) + tv_sqr(p1.x) + 
		tv_sqr(p1.y) + tv_sqr(p1.z) -
		2 * (sp.x*p1.x + sp.y*p1.y + sp.z * p1.z) -
		tv_sqr(r);
	i = a.y * a.y - 4 * a.x * a.z;

	if(i < 0.0f) {
		/* no intersection */
		pt.x = TV_INF;
		pt.y = TV_INF;
		pt.z = TV_INF;
	}
	else if(i == 0.0f) {
		tvfloat mu = -a.y / (2*a.x);
		pt.x = p1.x + mu * (p2.x - p1.x);
		pt.y = p1.y + mu * (p2.y - p1.y);
		pt.z = p1.z + mu * (p2.z - p1.z);
	}
	else {
		tvfloat mu;
		tvfloat sqrt_b = sqrt(a.y);
		/* first intersection */
		mu = (-a.y + sqrt_b - 4*a.x*a.z) / (2*a.x);
		pt.x = p1.x + mu*(p2.x-p1.x);
		pt.y = p1.y + mu*(p2.y-p1.y);
		pt.z = p1.z + mu*(p2.z-p1.z);
		/* second intersection */
		mu = (-a.y - sqrt_b - 4*a.x*a.z) / (2*a.x);
		pt.x = p1.x + mu*(p2.x-p1.x);
		pt.x = p1.y + mu*(p2.y-p1.y);
		pt.x = p1.z + mu*(p2.z-p1.z);
	}
	return pt;
}
/******************************************************************************
 * tv_physics_raycast
 * TODO:
 *****************************************************************************/
TV_physics_hit_info tv_physics_raycast(tv_vector3 src, tv_vector3 dir, tvfloat len)
{
	/* TODO: doesn't look at object's bounds, will take closest object by center
	 * point */	
	TV_collider **c;
	tv_vector3 s;
	tvfloat closest_distance = TV_INF;
	tv_vector3 dst;
	TV_physics_hit_info hit = {{TV_INF, TV_INF, TV_INF}, NULL};

	/* create the line collider to perform the raycast */
	tv_vector3_scale(&dir, len);
	tv_vector3_add(src, dir, &dst);

	/* for now, we're just gonna brute force this. 
	 * It would be easy to use some of the SAP junk in addition if speed is 
	 * ever an issue */
	for(c = (TV_collider**)utarray_front(colliders); 
		c != NULL;
		c = (TV_collider**)utarray_next(colliders, c)) {
		/* see if the ray intersects this collider, if it does, see if this is 
		 * now the closest collider
		 */
		tv_vector3 intersect;
		switch((*c)->type) {
		case TV_COLLIDER_BOX:
			/* TODO: */
			break;
		case TV_COLLIDER_SPHERE:			
			intersect = tv_physics_check_ray_sphere(src, dst, *c);
			break;
		case TV_COLLIDER_QUAD:
			intersect = tv_physics_check_ray_plane(src, dst, *c);
			break;
		}
		if(tv_vector3_distance(intersect, src) < closest_distance) {
			hit.collider = *c;
			hit.hit_loc = intersect;
		}
	}
	return hit;
}
/******************************************************************************
 * tv_collision_check
 * Checks for any collisions that are occuring with the given collider by a
 * brute-force search through the array of registered colliders.
 *****************************************************************************/
tv_array* tv_collision_check(TV_collider* c)
{
	tv_array *matches;
	TV_collider** it;
	tv_collision collision;
	
	utarray_new(matches, &ut_ptr_icd);
	/* check all collisions with the given collider */
	for(it = (TV_collider**)utarray_front(colliders);  it != NULL; it = (TV_collider**)utarray_next(colliders, it)) {
		if(tv_collider_check_collision(c, *it, &collision)) {
			tv_collision* col = (tv_collision*)tv_alloc(sizeof(tv_collision));
			*col = collision;
			utarray_push_back(matches, &col);
		}
	}
	return matches;
}