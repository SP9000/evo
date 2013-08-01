#include "collision.h"

typedef struct ColliderList {
	TvEntity* e;
	struct ColliderList* next;
	struct ColliderList* prev;
}ColliderList;

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


/* table of registered collider ID's and their OnCollide functions */
static DetectCollisionHash* registered_colliders; 
static OnCollideHash* registered_callbacks;

/* lists of all the colldiers sorted on various axes for fast detection */
static ColliderList* colliders;
static ColliderList* xSorted;

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

void tv_collision_add_entity(TvEntity* e)
{
	ColliderList* n = (ColliderList*)malloc(sizeof(ColliderList));
	LL_APPEND(xSorted, n);
	/* TODO //colliding = g_hash_table_new(NULL, NULL); */
}

void tv_collision_remove_entity(TvEntity* e)
{
	/* TODO: */
}

/* TODO: NO! This crap will not fly! */
void tv_collision_detect()
{
    TvEntity* c1;
    TvEntity* c2;
	ColliderList* it;
    ColliderList* jt;
	ColliderList* possibleCollisions = NULL;

    /* Check X */
    for(it = xSorted; it != NULL; it = it->next) {
		for(jt = it->next; jt != NULL; jt = jt->next) {
			c1 = it->e;
			c2 = jt->e;
			/* not overlapping? */
			if((c1->pos.x + c1->aabb.w) <
						(c2->pos.x)) {
				jt = it = it->next;
			}
			/* X is overlapping */
			else {
				if(c1->pos.y > c2->pos.y) {
					c1 = c2; 
					c2 = c1;
				}
				if(c1->aabb.w + c1->pos.y >= c2->pos.y) {
					/* Y is overlapping */
					if(c1->pos.z > c2->pos.z) {
						c1 = c2; 
						c2 = c1;
					}
					if(c1->aabb.d + c1->pos.z >= c2->pos.z) {
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

void tv_entity_collide(TvEntity* e, TvEntity* other)
{
	OnCollideHash* on_collide;
	DetectCollisionHash* detect;
	/* do precise collision detection */
	HASH_FIND_INT(registered_colliders, &e->collider, detect);
	if(detect) {
		/* call e's collide function */
		HASH_FIND_INT(registered_colliders, &e->collider, on_collide);
		if(on_collide) {
			on_collide->f(other);
		}
		/* call other's collide function */
		HASH_FIND_INT(registered_colliders, &other->collider, on_collide);
		if(on_collide) {
			on_collide->f(e);
		}
	}
}

void tv_collision_update()
{

}
