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