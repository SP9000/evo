#include "scene.h"

/* TODO: nice "spatial" representation we got here */
GSList* scene_layers[32];

void ForeachWidgetRecursive(Component_Widget* w, 
        void (*func)(Component_Widget*));

int Scene_Init()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        scene_layers[i] = NULL;
    }
    return 0;
}

int Scene_Quit()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        g_slist_free(scene_layers[i]);
    }
    return 0;
}

void Scene_Add(Component_Renderer* r)
{
    int i;
    scene_layers[r->layer] = g_slist_append(scene_layers[r->layer], (gpointer)r);
}
