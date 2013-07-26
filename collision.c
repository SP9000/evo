#include "collision.h"

/* table of registered collider ID's and their OnCollide functions */
static TvHashTable* registered_colliders; 
static TvHashTable* registered_callbacks;

TvHashTable* colliding = NULL;
static TvHashTable* collidingY = NULL;
static TvHashTable* collidingZ = NULL;

/* lists of all the colldiers sorted on various axes for fast detection */
static TvList* colliders;
static TvList* xSorted;
static TvList* ySorted;
static TvList* zSorted;

/* equality function for collision comparison */
gboolean ColEqual(gconstpointer a, gconstpointer b);

/* comparison functions for sorting */
static gint XCompare(gconstpointer a, gconstpointer b);
static gint YCompare(gconstpointer a, gconstpointer b);
static gint ZCompare(gconstpointer a, gconstpointer b);

void tv_collision_init()
{
    colliders = NULL;
	registered_colliders = g_hash_table_new(g_int_hash, g_int_equal);
	registered_callbacks = g_hash_table_new(g_int_hash, g_int_equal);
}

void tv_collision_register_collider(TvCollisionDetectFunc detect_func, tvuint id)
{
	g_hash_table_insert(registered_colliders, (gpointer)id, (gpointer)detect_func);
}

void tv_collision_register_component(TvCollisionCollideFunc on_collision, tvuint id)
{
	g_hash_table_insert(registered_callbacks, (gpointer)id, (gpointer)on_collision);
}

void tv_collision_add_entity(TvEntity* e)
{
    colliders = g_list_append(colliders, (gpointer)e);
    xSorted = g_list_insert_sorted(xSorted, (gpointer)e, XCompare);
    ySorted = g_list_insert_sorted(ySorted, (gpointer)e, YCompare);
    zSorted = g_list_insert_sorted(zSorted, (gpointer)e, ZCompare);
    colliding = g_hash_table_new(NULL, NULL);
}

void Collision_RemoveCollider(TvComponent* col)
{
    /* get that s*** outta' here */
    colliders = g_list_remove(colliders, (gpointer)col);
    xSorted = g_list_remove(xSorted, (gpointer)col);
    ySorted = g_list_remove(ySorted, (gpointer)col);
    zSorted = g_list_remove(zSorted, (gpointer)col);

}

void tv_collision_detect()
{
    GList* i;
    GList* j;
	GList* possibleCollisions;

    TvEntity* c1;
    TvEntity* c2;

    /* direct hash, custom equal func */
    collidingY = g_hash_table_new(NULL, ColEqual);
    collidingZ = g_hash_table_new(NULL, ColEqual);

    possibleCollisions = NULL;

    /* Check X */
    for(i = xSorted, j = g_list_next(xSorted); i != NULL; j = g_list_next(j)) {
        /* end of the list? */
        if(j == NULL) {
            j = i = g_list_next(i);
        }
        else {
            c1 = (TvEntity*)(i->data);
            c2 = (TvEntity*)(j->data);

            /* not overlapping? */
            if((c1->pos.x + c1->aabb.w) <
                     (c2->pos.x)) {
                j = i = g_list_next(i);
            }
            /* X is overlapping */
            else {
                possibleCollisions = g_list_append(possibleCollisions, (gpointer)c1);
            }
        }
    }
    /* Check Y */
    for(i = ySorted, j = g_list_next(ySorted); i != NULL; j = g_list_next(j)) {
        if(j == NULL) {
            j = i = g_list_next(i);
        }
        else {
            c1 = (TvEntity*)(i->data);
            c2 = (TvEntity*)(j->data);
            /* not overlapping? */
            if((c1->pos.y + c1->aabb.h) < 
                    (c2->pos.y)) {
                j = i = g_list_next(i);
            }
            /* overlapping Y */
            else {
                /* insert both this collision and the reverse */
                g_hash_table_insert(collidingY, (gpointer)c1, (gpointer)c2);
                g_hash_table_insert(collidingY, (gpointer)c2, (gpointer)c1);
            }
        }
    }

    /* Check Z */
    for(i = zSorted, j = g_list_next(zSorted); i != NULL; j = g_list_next(j)) {
        if(j == NULL) {
            j = i = g_list_next(i);
        }
        else {
            c1 = (TvEntity*)(i->data);
            c2 = (TvEntity*)(j->data);
            /* not overlapping? */
            if((c1->pos.z + c1->aabb.d) <
                    (c2->pos.z)) {
                j = i = g_list_next(i);
            }
            /* overlapping Z */
            else {
                /* insert both this collision and the reverse */
                g_hash_table_insert(collidingZ, (gpointer)c1, (gpointer)c2);
                g_hash_table_insert(collidingZ, (gpointer)c2, (gpointer)c1);
            }
        }
    }  
    
    /* check for matches on all axes, x axis overlap is implicit */
    for(i = possibleCollisions; i != NULL; i = g_list_next(i)) {
        c1 = (TvEntity*)i->data;
        /* check Y overlap */
        if(g_hash_table_lookup(collidingY, i->data) != NULL) {
            /* check Z overlap */
            c2 = (TvEntity*)g_hash_table_lookup(collidingZ, i->data);
            if(c2 != NULL) {
                /* are we already aware of this collision? */
                if(g_hash_table_lookup(colliding, (gpointer)c1) != (gpointer)c2) {
                    /* no, collision occurred for the first time. */
                    g_hash_table_insert(colliding, (gpointer)c1, (gpointer)c2);
                    g_hash_table_insert(colliding, (gpointer)c2, (gpointer)c1);
                    printf("collision between {%f,%f,%f} and {%f,%f,%f}\n", 
                            c1->pos.x, c1->pos.y, c1->pos.z,
                            c2->pos.x, c2->pos.y, c2->pos.z);
					tv_entity_collide(c1, c2);
                    fflush(stdout);
                }
            }
        }
    }
    /* colliding now contains all pairs of colliders who's AABB's are 
     * overlapping...do more precise detection TODO */
}

void tv_entity_collide(TvEntity* e, TvEntity* other)
{
	if(((TvCollisionDetectFunc)g_hash_table_lookup(registered_colliders, (gpointer)e->collide))(e)) {
		((TvCollisionCollideFunc)g_hash_table_lookup(registered_callbacks, (gpointer)e->collide))(other);
		((TvCollisionCollideFunc)g_hash_table_lookup(registered_callbacks, (gpointer)other->collide))(e);
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

gint YCompare(gconstpointer a, gconstpointer b)
{
    TvEntity* e1 = (TvEntity*)a;
    TvEntity* e2 = (TvEntity*)b;
    if(e1->pos.y < e2->pos.y) {
        return -1;
    }
    else if(e1->pos.y > e2->pos.y) {
        return 1;
    }
    return 0;
}

gint ZCompare(gconstpointer a, gconstpointer b)
{
    TvEntity* e1 = (TvEntity*)a;
    TvEntity* e2 = (TvEntity*)b;
    if(e1->pos.z < e2->pos.z) {
        return -1;
    }
    else if(e1->pos.z > e2->pos.z) {
        return 1;
    }
    return 0;
}

