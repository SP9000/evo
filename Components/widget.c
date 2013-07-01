/*****************************************************************************/
/* widget.c                                                                  */
/* This file contains definitions for functions used to compose the GUI.     */
/* This includes high level objects such as widgets, text boxes, etc.        */
/* No code to actually render said objects should be put here. As with       */
/* all other rendered items, that belongs in draw.h/draw.c                   */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: June 5, 2013                                                     */
/*****************************************************************************/
#ifndef COMPONENT_WIDGET
#define COMPONENT_WIDGET

/* flags that widget generates no events */
#define WIDGET_PLAIN     0x00000000
/* an event is generated when the widget is resized */
#define WIDGET_RESIZABLE 0x00000001
/* an event is generated each time the widget is moved via dragging */
#define WIDGET_DRAGABBLE 0x00000002  
/* an event is generated when the widget is clicked */
#define WIDGET_CLICKABLE 0x00000004

#include "../component.h"

/**
 * The widget structure.
 * Widgets are physically similar to models, but carry a little extra
 * information to assist in prettily laying out the GUI. When setting the
 * coordinates of the model within the widget, use a scale from 0 to 1 to allow
 * for flexible sizing on monitors of all resolutions.
 */
COMPONENT(Widget,
    /* the lower left corner and width/height of the widget */
    Rect rect;
    /* the minimum dimensions of the widget */
    float minW;
    float minH;
    /* maximum dimensions of the widget or 0 for no limit */
    float maxW;
    float maxH;
    /* flags for various attributes of the widget e.g. GUILAYOUT_RESIZABLE */
    uint32_t flags;
    /* the background that is displayed by this widget - not scrolled */
    struct Component_Model* background;
    /* the contents that this window displays - can be scrolled. */
    struct Component_Model* contents;
    /* the children of the widget - a list of widgets */
    GSList* children;
    /* the parent widget of this widget. NULL if this is the root widget */
    struct Component_Widget* parent;

    void (*AddWidget)(struct Component_Widget* self, 
        struct Component_Widget* p, float x, float y);
)

#ifdef BUILD
    /**
     * Add the specified widget to the specified parent widget. 
     * @param w the widget to add.
     * @param p the widget to parent this widget to, or NULL for root widget.
     * @param x the x location to add the widget at (range 0-1).
     * @param y the y location to add the widget at (range 0-1).
     */
    static void AddWidget(Component_Widget* self, Component_Widget* p, float x, float y)
    {
        self->children = g_slist_append(self->children, (gpointer)p);
    }
    /* helper function to free all of a widget's subwidgets */
    static void FreeWidgetRecursive(gpointer data, gpointer user_data)
    {
        Component_Widget* w = (Component_Widget*)data;

        w->contents->Free(w->contents);
        w->background->Free(w->background);

        /* recursively free all the subwidgets of this widget */
        if(w->children != NULL) {
            g_slist_foreach(w->children, FreeWidgetRecursive, NULL);
        }
        free(w);
    }
    /**
     * Remove the specified widget from the GUILayout system.
     * The widget is deleted upon removal and its resources freed.
     * @param w the widget to remove.
     */
    static void RemoveWidget(Component_Widget* self)
    {
        /* remove link from parent to this widget */
        if(self->parent) {
            self->parent->children = g_slist_remove(self->parent->children, (gpointer)self);
        }
        /* remove this widget and all its children */
        FreeWidgetRecursive((gpointer)self->children, NULL);
    }

    static void Start() 
    {

    }
    static void Update()
    {

    }
    static void Collide()
    {

    }
#endif
BEGIN(Widget, Component_Widget* parent)
CTOR(
    self->AddWidget = AddWidget;
)
END
#endif
