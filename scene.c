#include "scene.h"

/* TODO: nice "spatial" representation we got here */
static GSList* scene;
static Component_Widget* guiRoot;

void ForeachWidgetRecursive(Component_Widget* w, 
        void (*func)(Component_Widget*));

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

void Scene_Add(Component_Model* m)
{
    scene = g_slist_append(scene, (gpointer)m);
}

void Scene_AddWidget(Component_Widget* w)
{
    /* if parent is NULL, make this widget the new root widget */
    if(w->parent == NULL) {
        guiRoot = w;
    }
    /* parent is given, add this widget to that parent's children */
    else {
        w->parent->children = g_slist_append(w->parent->children, w);
    }
}

void Scene_Foreach(void (*func)(Component_Model*))
{
    GSList* it;
    for(it = scene; it != NULL; it = g_slist_next(it)) {
        func((Component_Model*)(it->data));
    }
}

void Scene_ForeachWidget(void (*func)(Component_Widget*))
{
    /* no GUI */
    if(guiRoot == NULL) {
        return;
    }

    /* recursively call func on all the widgets */
    ForeachWidgetRecursive(guiRoot, func);
}

void ForeachWidgetRecursive(Component_Widget* w, 
        void (*func)(Component_Widget*))
{
    GSList* it;
    for(it = w->children; it != NULL; it = g_slist_next(it)) {
        Component_Widget* next = (Component_Widget*)(it->data);
        ForeachWidgetRecursive(next, func);
    }
    func(w);
}

