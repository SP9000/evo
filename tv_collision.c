#include "tv_collision.h"

typedef struct OnCollideHash {
	tvuint cid;
	TvCollisionCollideFunc f;
	TvHashHandle hh;
}OnCollideHash;

typedef struct DetectCollisionHash {
	tvuint cid;
	TvCollisionDetectFunc f;
	TvHashHandle hh;
}DetectCollisionHash;

void tv_entity_collide(tv_entity* e, tv_entity* other);

/* table of registered collider ID's and their OnCollide functions */
static DetectCollisionHash* registered_colliders; 
static OnCollideHash* registered_callbacks;

/* lists of all the colldiers sorted on various axes for fast detection */
static UT_array *colliders;
static UT_array *xSorted;

void tv_collision_init()
{
    colliders = NULL;
	registered_colliders = NULL;
	registered_callbacks = NULL;
}

void tv_collision_register_collider(TvCollisionDetectFunc detect_func, tvuint id)
{
	DetectCollisionHash* entry = (DetectCollisionHash*)malloc(sizeof(DetectCollisionHash));
	entry->cid = id;
	entry->f = detect_func;
	HASH_ADD_INT(registered_colliders, cid, entry);
}

void tv_collision_register_component(TvCollisionCollideFunc on_collision, tvuint id)
{
	OnCollideHash* entry = (OnCollideHash*)malloc(sizeof(OnCollideHash));
	entry->cid = id;
	entry->f = on_collision;
	HASH_ADD_INT(registered_callbacks, cid, entry);
}

void tv_collision_add_entity(tv_entity* e)
{
	utarray_push_back(colliders, e);
	/* TODO //colliding = g_hash_table_new(NULL, NULL); */
}

void tv_collision_remove_entity(tv_entity* e)
{
	utarray_erase(colliders,
		utarray_eltidx(colliders, e),
		1);
}

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
}

void tv_entity_collide(tv_entity* e, tv_entity* other)
{
	OnCollideHash* on_collide;
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

void tv_collision_update()
{

}

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
	s.x = s.y = s.z = len;
	dst = tv_vector3_add(src, tv_vector3_scale(dir, s));

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
		}
		if(tv_vector3_distance(intersect, src) < closest_distance) {
			hit.collider = *c;
			hit.hit_loc = intersect;
		}
	}
	return hit;
}