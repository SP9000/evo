#include "gamestate.h"

static QTree tree;
static Model *models;

Gamestate *GamestateNew()
{
    Gamestate *gs = malloc(sizeof(Gamestate));
    gs->entities = g_hash_table_new_full(g_str_hash(), g_str_equal(), NULL, EntityFree);
    return gs;
}
void Gamestate *GamestateFree(Gamestate *gs)
{
    g_hash_table_destroy(gs->entities);
    free(gs);
}

void GamestateAddEntity(Gamestate *gs, Entity *e, char *name) 
{
    g_hash_table_insert(gs, name, e);
}

void FreeEntityIterator(gpointer key, gpointer value, gpointer user_data)
{
    
}

