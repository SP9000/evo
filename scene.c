#include "scene.h"

/* TODO: nice "spatial" representation we got here */
GSList* scene_layers[32];

int tv_scene_init()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        scene_layers[i] = NULL;
    }
    return 0;
}

int tv_scene_quit()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        g_slist_free(scene_layers[i]);
    }
    return 0;
}

void tv_scene_add(Component_Renderer* r)
{
    scene_layers[r->layer] = g_slist_append(scene_layers[r->layer], (gpointer)r);
}
