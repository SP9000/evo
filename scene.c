#include "scene.h"

/* TODO: nice "spatial" representation we got here */
GSList* scene_layers[32];

TvHashTable* renderers_table;
TvHashTable* renderer_layer_table;

int tv_scene_init()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        scene_layers[i] = NULL;
    }
    return 0;
	renderers_table = g_hash_table_new(g_int_hash, g_int_equal);
	renderer_layer_table = g_hash_table_new(g_int_hash, g_int_equal);
}

int tv_scene_quit()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        g_slist_free(scene_layers[i]);
    }
    return 0;
}
void tv_scene_register_renderer(void (*render_func)(TvComponent*), tvuint layer, tvuint id)
{
	g_hash_table_insert(renderers_table, (gpointer)id, (gpointer)render_func);
	g_hash_table_insert(renderer_layer_table, (gpointer)id, (gpointer)layer);
}

void tv_scene_add(TvComponent* c)
{
	tvuint layer = (tvuint)g_hash_table_lookup(renderer_layer_table, (gpointer)c->id);
    scene_layers[layer] = g_slist_append(scene_layers[layer], (gpointer)c);
}
