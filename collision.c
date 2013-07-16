#include "collision.h"

GHashTable* colliding = NULL;
static GHashTable* collidingY = NULL;
static GHashTable* collidingZ = NULL;

/* lists of all the colldiers sorted on various axes for fast detection */
static GList* colliders;
static GList* xSorted;
static GList* ySorted;
static GList* zSorted;

/* equality function for collision comparison */
gboolean ColEqual(gconstpointer a, gconstpointer b);

/* comparison functions for sorting */
static gint XCompare(gconstpointer a, gconstpointer b);
static gint YCompare(gconstpointer a, gconstpointer b);
static gint ZCompare(gconstpointer a, gconstpointer b);


void Collision_Init()
{
    colliders = NULL;
}

void Collision_AddCollider(Component_Collider* col)
{
    colliders = g_list_append(colliders, col);
    xSorted = g_list_insert_sorted(xSorted, (gpointer)col, XCompare);
    ySorted = g_list_insert_sorted(ySorted, (gpointer)col, YCompare);
    zSorted = g_list_insert_sorted(zSorted, (gpointer)col, ZCompare);
    colliding = g_hash_table_new(NULL, NULL);
}

void Collision_RemoveCollider(Component_Collider* col)
{
    /* get that s*** outta' here */
    colliders = g_list_remove(colliders, (gpointer)col);
    xSorted = g_list_remove(xSorted, (gpointer)col);
    ySorted = g_list_remove(ySorted, (gpointer)col);
    zSorted = g_list_remove(zSorted, (gpointer)col);

}

void Collision_Detect()
{
    GList* i;
    GList* j;

    Component_Collider* c1;
    Component_Collider* c2;

    /* direct hash, custom equal func */
    collidingY = g_hash_table_new(NULL, ColEqual);
    collidingZ = g_hash_table_new(NULL, ColEqual);

    GList* possibleCollisions = NULL;

    /* Check X */
    for(i = xSorted, j = g_list_next(xSorted); i != NULL; j = g_list_next(j)) {
        /* end of the list? */
        if(j == NULL) {
            j = i = g_list_next(i);
        }
        else {
            c1 = (Component_Collider*)(i->data);
            c2 = (Component_Collider*)(j->data);

            /* not overlapping? */
            if((c1->transform->pos.x + c1->aabb.w) <
                     (c2->transform->pos.x)) {
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
            c1 = (Component_Collider*)(i->data);
            c2 = (Component_Collider*)(j->data);
            /* not overlapping? */
            if((c1->transform->pos.y + c1->aabb.h) < 
                    (c2->transform->pos.y)) {
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
            c1 = (Component_Collider*)(i->data);
            c2 = (Component_Collider*)(j->data);
            /* not overlapping? */
            if((c1->transform->pos.z + c1->aabb.d) <
                    (c2->transform->pos.z)) {
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
        c1 = (Component_Collider*)i->data;
        /* check Y overlap */
        if(g_hash_table_lookup(collidingY, i->data) != NULL) {
            /* check Z overlap */
            c2 = (Component_Collider*)g_hash_table_lookup(collidingZ, i->data);
            if(c2 != NULL) {
                /* are we already aware of this collision? */
                if(g_hash_table_lookup(colliding, (gpointer)c1) != (gpointer)c2) {
                    /* no, collision occurred for the first time. */
                    g_hash_table_insert(colliding, (gpointer)c1, (gpointer)c2);
                    g_hash_table_insert(colliding, (gpointer)c2, (gpointer)c1);
                    printf("collision between {%f,%f,%f} and {%f,%f,%f}\n", 
                            c1->transform->pos.x, c1->transform->pos.y, c1->transform->pos.z,
                            c2->transform->pos.x, c2->transform->pos.y, c2->transform->pos.z);
                    Entity_Collide(c1->entity, c2->entity);
                    fflush(stdout);
                }
            }
        }
    }
    /* colliding now contains all pairs of colliders who's AABB's are 
     * overlapping...do more precise detection TODO */
    
}

void Collision_Update()
{

}


gboolean ColEqual(gconstpointer a, gconstpointer b)
{
    Collision* c1 = (Collision*)a;
    Collision* c2 = (Collision*)b;
    if(((c1->col1 == c2->col1) && (c1->col2 == c2->col2)) ||
            ((c1->col2 == c2->col1) && (c1->col1 == c2->col2))) {
        return TRUE;
    }
    return FALSE;
}

gint XCompare(gconstpointer a, gconstpointer b)
{
    Component_Collider* c1 = (Component_Collider*)a;
    Component_Collider* c2 = (Component_Collider*)b;
    if(c1->transform->pos.x < c2->transform->pos.x) {
        return -1;
    }
    else if(c1->transform->pos.x > c2->transform->pos.x) {
        return 1;
    }
    else {
        return 0;
    }
}

gint YCompare(gconstpointer a, gconstpointer b)
{
    Component_Collider* c1 = (Component_Collider*)a;
    Component_Collider* c2 = (Component_Collider*)b;
    if(c1->transform->pos.y < c2->transform->pos.y) {
        return -1;
    }
    else if(c1->transform->pos.y > c2->transform->pos.y) {
        return 1;
    }
    return 0;
}

gint ZCompare(gconstpointer a, gconstpointer b)
{
    Component_Collider* c1 = (Component_Collider*)a;
    Component_Collider* c2 = (Component_Collider*)b;
    if(c1->transform->pos.z < c2->transform->pos.z) {
        return -1;
    }
    else if(c1->transform->pos.z > c2->transform->pos.z) {
        return 1;
    }
    return 0;
}

