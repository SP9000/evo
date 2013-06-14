#include "guilayout.h"

static void FreeWidgetRecursive(gpointer data, gpointer user_data);
static Widget* root_widget;

void GUILayoutInit()
{
    root_widget = NULL;
}

void GUILayoutSetRootWidget(Widget* w)
{
    root_widget = w;
}

Widget* GUILayoutGetRootWidget() 
{
    return root_widget;
}

void GUILayoutAddWidget(Widget* w, Widget* p, float x, float y)
{
    w->children = g_slist_append(w->children, (gpointer)p);
}

void GUILayoutRemoveWidget(Widget* w)
{
    /* remove link from parent to this widget */
    if(w->parent) {
        w->children = g_slist_remove(w->parent->children, (gpointer)w);
    }
    /* remove this widget and all its children */
    FreeWidgetRecursive((gpointer)w->children, NULL);
}

Widget* GUILayoutNewWidget(Model* background, Model* contents, Rect* r, uint32_t flags)
{
    Widget* w = (Widget*)malloc(sizeof(Widget));
    w->contents = contents;
    w->background = background;
    w->flags = flags;
    w->children = NULL;

    w->rect.x = r->x;
    w->rect.y = r->y;
    w->rect.w = r->w;
    w->rect.h = r->h;

    return w;
}

void FreeWidgetRecursive(gpointer data, gpointer user_data)
{
    Widget* w = (Widget*)data;

    ModelFree(w->contents);
    ModelFree(w->background);

    /* recursively free all the subwidgets of this widget */
    if(w->children != NULL) {
        g_slist_foreach(w->children, FreeWidgetRecursive, NULL);
    }
    free(w);
}

Widget* GUILayoutNewTextBox(Model* background, Rect* r, char* text)
{
    /* TODO: */
    float textW = r->w * 400;
    float textH = r->h * 200;

    Widget* w = (Widget*)malloc(sizeof(Widget));
    Model* text_model = GenText(text, textW, textH); 
    return GUILayoutNewWidget(background, text_model, r, GUILAYOUT_PLAIN);
}

