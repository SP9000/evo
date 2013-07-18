/*****************************************************************************/
/* Widget.c                                                                  */
/* This file contains definitions for functions used to compose the GUI.     */
/* No code to actually render widgets should be put here. As with            */
/* all other rendered items, that belongs in draw.h/draw.c                   */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: June 5, 2013                                                     */
/*****************************************************************************/

/* TODO: maybe #ifdef Widget and make a separate version for when mouse input
 * is not needed (fewer or no default callbacks for input-events)
 */

void FreeWidgetRecursive(gpointer data, gpointer user_data);

COMPONENT Widget {
    /*************************************************************************/
    /*                            Data                                       */
    /*************************************************************************/
    /* the lower left corner and width/height of the widget */
    public Rect rect;
    /* the minimum dimensions of the widget */
    public float minW;
    public float minH;
    /* maximum dimensions of the widget or 0 for no limit */
    public float maxW;
    public float maxH;
    /* flags for various attributes of the widget TODO: needed? */
    public uint32_t flags;
    /* the background that is displayed by this widget - not scrolled */
    public Component_Model* background;
    /* the contents that this window displays - list of models */
    public GSList* contents;
    /* the children of the widget - a list of widgets */
    public GSList* children;
    /* the parent widget of this widget. NULL if this is the root widget */
    public Component_Widget* parent;

    /*************************************************************************/
    /*                            Callbacks                                  */
    /*************************************************************************/
    /** 
     * Called when this widget becomes focused (ususally by being clicked).
     */
    public void (*OnFocus)();
    /**
     * Called when the mouse is moved over this widget.
     * Even if the widget is not focused, this function is called on it.
     * The coordinates given are from 0-1 as with all widget functions.
     * @param 1 the relative x position of the mouse from the lower-left.
     * @param 2 the relative y position of the mouse from the lower-right.
     */
    public void (*OnMoveOver)(float, float);
    /**
     * Called when the left mouse button is pressed on this widget.
     * The coordinates given are from 0-1 as with all widget functions.
     * @param 1 the relative x position of the mouse from the lower-left.
     * @param 2 the relative y position of the mouse from the lower-right.
     */
    public void (*OnLClick)(float, float);
    /**
     * Called when the right mouse button is pressed on this widget.
     * The coordinates given are from 0-1 as with all widget functions.
     * @param 1 the relative x position of the mouse from the lower-left.
     * @param 2 the relative y position of the mouse from the lower-right.
     */
    public void (*OnRClick)(float, float);
    /**
     * Called when the scroll-wheel is moved while this widget is focused.
     * @param 1 the amount the wheel was scrolled (up is positive).
     */
    public void (*OnScroll)(int);
    /**
     * Called when a key is pressed down while this widget is focused.
     * @param 1 the key that was pressed.
     */
    public void (*OnKeyDown)(int);
    /**
     * Called when a key is released while this widget is focused.
     * @param 1 the key that was released.
     */
    public void (*OnKeyUp)(int);

    /*************************************************************************/
    /*                            Functions                                  */
    /*************************************************************************/
    /**
     * Add the specified widget to this widget's children.
     * @param w the widget to add.
     * @param x the x location to add the widget at (range 0-1).
     * @param y the y location to add the widget at (range 0-1).
     */
    public void AddWidget(Component_Widget* w, float x, float y)
    {
        self->children = g_slist_append(self->children, (gpointer)w);
    }

    /**
     * Remove and free the resources of this widget and all its children.
     */
    public void RemoveWidget()
    {
        /* remove link from parent to this widget */
        if(self->parent) {
            self->parent->children = g_slist_remove(self->parent->children, 
                    (gpointer)self);
        }
        /* remove this widget and all its children */
        FreeWidgetRecursive((gpointer)self->children, NULL);
    }

    /*************************************************************************/
    /*                        Default Functions                              */
    /*************************************************************************/
    /**
     * Start this component.
     * Called when the entity this is attached to is "Start"ed.
     */
    void Start() 
    {

    }
    /**
     * Update the component.
     * Called every frame.
     */
    void Update()
    {

    }
    /**
     * Handle collision with another entity.
     * @param other the other colliding entity.
     */
    void Collide(Entity* other)
    {

    }
}
/*****************************************************************************/

/* helper function to free all of a widget's subwidgets */
void FreeWidgetRecursive(gpointer data, gpointer user_data)
{
    Component_Widget* w = (Component_Widget*)data;
    GSList* it; 

    for(it = w->contents; it != NULL; it = g_slist_next(it)) {
        Component_Model* m = (Component_Model*)it->data;
        m->Free(m);
    }
    w->background->Free(w->background);

    /* recursively free all the subwidgets of this widget */
    if(w->children != NULL) {
        g_slist_foreach(w->children, FreeWidgetRecursive, NULL);
    }
    free(w);
}

