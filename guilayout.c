#include "guilayout.h"

static void FreeWidgetRecursive(gpointer data, gpointer user_data);
static Widget* root_widget;
static Material* guiMaterial;

int GUILayout_Init()
{
    root_widget = NULL;
    return 0;
}

void GUILayout_SetMaterial(Material* mat)
{
    guiMaterial = mat;
}

void GUILayout_SetRootWidget(Widget* w)
{
    root_widget = w;
}

Widget* GUILayout_GetRootWidget() 
{
    return root_widget;
}

void GUILayout_AddWidget(Widget* w, Widget* p, float x, float y)
{
    w->children = g_slist_append(w->children, (gpointer)p);
}

void GUILayout_RemoveWidget(Widget* w)
{
    /* remove link from parent to this widget */
    if(w->parent) {
        w->children = g_slist_remove(w->parent->children, (gpointer)w);
    }
    /* remove this widget and all its children */
    FreeWidgetRecursive((gpointer)w->children, NULL);
}

Widget* GUILayout_NewWidget(Model* background, Model* contents, Rect* r, uint32_t flags)
{
    Widget* w = (Widget*)malloc(sizeof(Widget));
    w->contents = contents;
    w->background = background;
    w->flags = flags;
    w->children = NULL;

    w->rect = *r;

    return w;
}

Widget* GUILayout_NewTextBox(Model* background, Rect* r, char* text)
{
    /* TODO: */
    float textW = r->w * 400;
    float textH = r->h * 200;

    Widget* w = (Widget*)malloc(sizeof(Widget));
    Model* text_model = GenText(text, textW, textH); 
    Model_SetMaterial(text_model, guiMaterial);
    return GUILayout_NewWidget(background, text_model, r, GUILAYOUT_PLAIN);
}

/*****************************************************************************/
/*                           local functions                                 */
/*****************************************************************************/
void FreeWidgetRecursive(gpointer data, gpointer user_data)
{
    Widget* w = (Widget*)data;

    Model_Free(w->contents);
    Model_Free(w->background);

    /* recursively free all the subwidgets of this widget */
    if(w->children != NULL) {
        g_slist_foreach(w->children, FreeWidgetRecursive, NULL);
    }
    free(w);
}

