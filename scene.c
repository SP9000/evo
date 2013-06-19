#include "scene.h"

/* TODO: nice "spatial" representation we got here */
static GSList* scene;

int Scene_Init()
{
    scene = NULL;
    return 0;
}

int Scene_Quit()
{
    g_slist_free(scene);
    return 0;
}

void Scene_Add(Model* m)
{
    scene = g_slist_append(scene, (gpointer)m);
}

void Scene_Foreach(void (*func)(Model*))
{
    GSList* it;
    for(it = scene; it != NULL; it = g_slist_next(it)) {
        func((Model*)(it->data));
    }
}

