#include "collision.h"

typedef struct ColliderList {
	TvEntity* e;
	ColliderList* next;
	ColliderList* prev;
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

/* equality function for collision comparison */
gboolean ColEqual(gconstpointer a, gconstpointer b);

/* comparison functions for sorting */
static gint XCompare(gconstpointer a, gconstpointer b);

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
	ColliderList* node = (ColliderList*)malloc(sizeof(ColliderList));

    /* get that s*** outta' here */
	LL_SEARCH(xSorted, node, e, XCompare);
	LL_DELETE(xSorted, node);
}

void tv_collision_detect()
{
	ColliderList* it;
    ColliderList* jt;
	ColliderList* possibleCollisions;
	
	TvCollision* col;

    TvEntity* c1;
    TvEntity* c2;


    possibleCollisions = NULL;

    /* Check X */
    for(it = xSorted, jt = it->next; it != NULL; jt = jt->next) {
        /* end of the list? */
        if(jt == NULL) {
            jt = it = it->next;
        }
        else {
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

gboolean ColEqual(gconstpointer a, gconstpointer b)
{
    TvCollision* c1 = (TvCollision*)a;
    TvCollision* c2 = (TvCollision*)b;
    if(((c1->col1 == c2->col1) && (c1->col2 == c2->col2)) ||
            ((c1->col2 == c2->col1) && (c1->col1 == c2->col2))) {
        return TRUE;
    }
    return FALSE;
}

gint XCompare(gconstpointer a, gconstpointer b)
{
    TvEntity* e1 = (TvEntity*)a;
    TvEntity* e2 = (TvEntity*)b;
    if(e1->pos.x < e2->pos.x) {
        return -1;
    }
    else if(e1->pos.x > e2->pos.x) {
        return 1;
    }
    else {
        return 0;
    }
}
