#include "collision.h"


/* flags for each axis the object is colliding on */
typedef struct tagColliding {
    bool x, y, z;
}Colliding;

GList* colliding = NULL;
static GHashTable* collidingX = NULL;
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


void CollisionInit()
{
    colliders = NULL;
}

void CollisionAddCollider(Collider* col)
{
    colliders = g_list_append(colliders, col);
    xSorted = g_list_insert_sorted(xSorted, (gpointer)col, XCompare);
    ySorted = g_list_insert_sorted(ySorted, (gpointer)col, YCompare);
    zSorted = g_list_insert_sorted(zSorted, (gpointer)col, ZCompare);
}

void CollisionRemoveCollider(Collider* col)
{
    /* get that s*** outta' here */
    colliders = g_list_remove(colliders, (gpointer)col);
    xSorted = g_list_remove(xSorted, (gpointer)col);
    ySorted = g_list_remove(ySorted, (gpointer)col);
    zSorted = g_list_remove(zSorted, (gpointer)col);
}

void CollisionDetect()
{
    GList* i;
    GList* j;

    /* direct hashing (default) can be used because each collision will have
       a separate entry in the hash table this way. */
    collidingX = g_hash_table_new(NULL, ColEqual);
    collidingY = g_hash_table_new(NULL, ColEqual);
    collidingZ = g_hash_table_new(NULL, ColEqual);

    GList* possibleCollisions = NULL;

    /* Check X */
    for(i = xSorted, j = g_list_next(xSorted); g_list_next(i) != NULL; ) {
        /* overlapping X? */
        if(((Collider*)i)->pos->x + ((Collider*)i)->aabb->w > ((Collider*)g_list_next(i))->pos->x) {
            Collision* c = (Collision*)malloc(sizeof(Collision));
            c->col1 = (Collider*)i;
            c->col2 = (Collider*)g_list_next(i);
            g_hash_table_insert(collidingX, (gpointer)i, (gpointer)c);
            possibleCollisions = g_list_append(possibleCollisions, (gpointer)i);
        }
        /* not overlapping, move on */
        else {
            i = g_list_next(i);
            j = g_list_next(i);
        }
    }
    /* Check Y */
    for(i = ySorted, j = g_list_next(ySorted); i != NULL; ) {
        /* overlapping Y? */
        if(((Collider*)i)->pos->y + ((Collider*)i)->aabb->h > ((Collider*)g_list_next(i))->pos->y) {
            Collision* c = (Collision*)malloc(sizeof(Collision));
            c->col1 = (Collider*)i;
            c->col2 = (Collider*)g_list_next(i);
            g_hash_table_insert(collidingY, (gpointer)i, (gpointer)c);
            //???g_list_append(possibleCollisions, (gpointer)i, (gpointer)c);
            j = g_list_next(j);
        }
        /* not overlapping, move on */
        else {
            i = g_list_next(i);
            j = g_list_next(i);
        }
    }
    /* Check Z */
    for(i = zSorted, j = g_list_next(zSorted); i != NULL; ) {
        /* overlapping Z? */
        if(((Collider*)i)->pos->z + ((Collider*)i)->aabb->d > ((Collider*)g_list_next(i))->pos->z) {
            Collision* c = (Collision*)malloc(sizeof(Collision));
            c->col1 = (Collider*)i;
            c->col2 = (Collider*)g_list_next(i);
            g_hash_table_insert(collidingY, (gpointer)i, (gpointer)c);
            j = g_list_next(j);
        }
        /* not overlapping, move on */
        else {
            i = g_list_next(i);
            j = g_list_next(i);
        }
    }

    /* check for matches on all axes, x axis overlap is implicit */
    for(i = possibleCollisions; i != NULL; i = g_list_next(i)) {
        GSList* found;
        GSList* possible_x;
        /* check Y overlap */
        if(g_hash_table_lookup(collidingY, i) != NULL) {
            /* check Z overlap */
            if(g_hash_table_lookup(collidingZ, i) != NULL) {
                /* collision occurred */
                colliding = g_list_append(colliding, (gpointer)i);
            }
        }
    }
}

void CollisionUpdate()
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
    Collider* c1 = (Collider*)a;
    Collider* c2 = (Collider*)b;
    if(c1->pos->x < c1->pos->x) {
        return -1;
    }
    else if(c1->pos->x > c1->pos->x) {
        return 1;
    }
    else {
        return 0;
    }
}

gint YCompare(gconstpointer a, gconstpointer b)
{
    Collider* c1 = (Collider*)a;
    Collider* c2 = (Collider*)b;
    if(c1->pos->y < c1->pos->y) {
        return -1;
    }
    else if(c1->pos->y > c1->pos->y) {
        return 1;
    }
    else {
        return 0;
    }
}

gint ZCompare(gconstpointer a, gconstpointer b)
{
    Collider* c1 = (Collider*)a;
    Collider* c2 = (Collider*)b;
    if(c1->pos->y < c1->pos->y) {
        return -1;
    }
    else if(c1->pos->y > c1->pos->y) {
        return 1;
    }
    else {
        return 0;
    }
}

